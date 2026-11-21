import numpy as np
import random
import re
import unittest
from types import SimpleNamespace

from opendbc.can import CANPacker, CANParser
from opendbc.can.dbc import DBC as DBCParsed
from opendbc.car import Bus, DT_CTRL, structs
from opendbc.car.structs import CarParams
from opendbc.car.fw_versions import match_fw_to_car, match_fw_to_car_exact
from opendbc.car.vin import VIN_UNKNOWN
from opendbc.car.volkswagen import mqbcan
from opendbc.car.volkswagen.carcontroller import CarController, HCAMitigation
from opendbc.car.volkswagen.carstate import CarState, HCA_AUTOSTOP_GRACE_FRAMES
from opendbc.car.volkswagen.interface import CarInterface
from opendbc.car.volkswagen.values import (CAR, DBC, CanBus, CarControllerParams as CCP, FW_QUERY_CONFIG, WMI,
                                            VolkswagenFlags, VolkswagenSafetyFlags)
from opendbc.car.volkswagen.fingerprints import FW_VERSIONS

Ecu = CarParams.Ecu

CHASSIS_CODE_PATTERN = re.compile('[A-Z0-9]{2}')
# TODO: determine the unknown groups
SPARE_PART_FW_PATTERN = re.compile(b'\xf1\x87(?P<gateway>[0-9][0-9A-Z]{2})(?P<unknown>[0-9][0-9A-Z][0-9])(?P<unknown2>[0-9A-Z]{2}[0-9])([A-Z0-9]| )')


class TestVolkswagenHCAMitigation(unittest.TestCase):
  STUCK_TORQUE_FRAMES = round(CCP.STEER_TIME_STUCK_TORQUE / (DT_CTRL * CCP.STEER_STEP))

  def test_same_torque_mitigation(self):
    """Same-torque nudge fires at the threshold, in the correct direction, and resets cleanly."""
    hca_mitigation = HCAMitigation(CCP)

    for actuator_value in (-CCP.STEER_MAX, -1, 0, 1, CCP.STEER_MAX):
      hca_mitigation.update(0, 0)  # Reset mitigation state
      for frame in range(self.STUCK_TORQUE_FRAMES + 2):
        should_nudge = actuator_value != 0 and frame == self.STUCK_TORQUE_FRAMES
        expected_torque = actuator_value - (1, -1)[actuator_value < 0] if should_nudge else actuator_value
        assert hca_mitigation.update(actuator_value, actuator_value) == expected_torque, f"{frame=}"

class TestVolkswagenPlatformConfigs(unittest.TestCase):
  MEB_BATCH = (CAR.VOLKSWAGEN_ID3_MK1, CAR.VOLKSWAGEN_ID3_MK2,
               CAR.AUDI_Q4_MK1, CAR.AUDI_Q4_MK2,
               CAR.SKODA_ENYAQ_MK1, CAR.SKODA_ENYAQ_MK2)

  def test_meb_firmware_only_ambiguity_stays_within_generation(self):
    # Shared radar part numbers cannot identify the body/chassis without VIN.
    for car_model in self.MEB_BATCH:
      with self.subTest(car_model=car_model):
        live = {ecu[1:]: set(versions) for ecu, versions in FW_VERSIONS[car_model].items()}
        matches = match_fw_to_car_exact(live, "volkswagen")
        self.assertIn(car_model, matches)
        self.assertTrue(all(CAR[name].config.flags & VolkswagenFlags.MEB for name in matches))
        self.assertTrue(all(bool(CAR[name].config.flags & VolkswagenFlags.MEB_GEN2) ==
                            bool(car_model.config.flags & VolkswagenFlags.MEB_GEN2) for name in matches))

  def test_meb_production_matcher_requires_matching_vin(self):
    for car_model in (*self.MEB_BATCH, CAR.VOLKSWAGEN_ID4_MK1, CAR.VOLKSWAGEN_ID4_MK2, CAR.CUPRA_BORN_MK1):
      with self.subTest(car_model=car_model):
        car_fw = [CarParams.CarFw(ecu=ecu, address=addr, subAddress=0 if sub is None else sub,
                                 fwVersion=versions[0], brand="volkswagen")
                  for (ecu, addr, sub), versions in FW_VERSIONS[car_model].items()]
        vin = ["0"] * 17
        vin[:3] = next(iter(car_model.config.wmis))
        vin[6:8] = next(iter(car_model.config.chassis_codes))
        vin[9] = next(iter(car_model.config.model_years)) if car_model.config.model_years else "M"
        raw_exact = match_fw_to_car_exact({(addr, sub): {versions[0]} for (_, addr, sub), versions in FW_VERSIONS[car_model].items()},
                                          "volkswagen")
        exact, matches = match_fw_to_car(car_fw, "".join(vin), log=False)
        self.assertEqual(exact, len(raw_exact) == 1)
        self.assertEqual(matches, {car_model})

        for unknown_vin in (VIN_UNKNOWN, "bad", "".join(vin[:6] + ["0", "0"] + vin[8:])):
          exact, unresolved = match_fw_to_car(car_fw, unknown_vin, log=False)
          self.assertTrue(exact)
          self.assertEqual(unresolved, raw_exact)

    # A newer VIN cannot turn older-generation firmware into a candidate.
    older = CAR.VOLKSWAGEN_ID3_MK1
    old_fw = [CarParams.CarFw(ecu=ecu, address=addr, subAddress=0 if sub is None else sub,
                             fwVersion=versions[0], brand="volkswagen")
              for (ecu, addr, sub), versions in FW_VERSIONS[older].items()]
    mismatched = ["0"] * 17
    mismatched[:3] = "WVW"
    mismatched[6:8] = "E1"
    mismatched[9] = "R"
    exact, unresolved = match_fw_to_car(old_fw, "".join(mismatched), log=False)
    self.assertTrue(exact)
    self.assertGreater(len(unresolved), 1)
    self.assertNotIn(CAR.VOLKSWAGEN_ID3_MK2, unresolved)

  def test_meb_generation_years_disambiguate_shared_chassis(self):
    pairs = ((CAR.VOLKSWAGEN_ID3_MK1, CAR.VOLKSWAGEN_ID3_MK2),
             (CAR.AUDI_Q4_MK1, CAR.AUDI_Q4_MK2),
             (CAR.SKODA_ENYAQ_MK1, CAR.SKODA_ENYAQ_MK2))
    for older, newer in pairs:
      for expected, year in ((older, next(iter(older.config.model_years))),
                             (newer, next(iter(newer.config.model_years))), (None, "0")):
        with self.subTest(older=older, newer=newer, year=year):
          vin = ["0"] * 17
          vin[:3] = next(iter(older.config.wmis))
          vin[6:8] = next(iter(older.config.chassis_codes))
          vin[9] = year
          radar_fw = FW_VERSIONS[older][Ecu.fwdRadar, 0x757, None][0]
          matches = FW_QUERY_CONFIG.match_fw_to_car_fuzzy({(0x757, None): [radar_fw]}, "".join(vin), FW_VERSIONS)
          self.assertEqual(matches, {expected} if expected is not None else set())

  def test_id4_chassis_year_and_radar_firmware_match(self):
    radar_fw = FW_VERSIONS[CAR.VOLKSWAGEN_ID4_MK1][Ecu.fwdRadar, 0x757, None][0]
    for wmi in ("1V2", "WVW", "WVG", "XXX"):
      for chassis in ("E2", "E8"):
        for year in ("M", "N", "P", "R", "S", "L", "T", "0"):
          vin = list("00000000000000000")
          vin[:3], vin[6:8], vin[9] = wmi, chassis, year
          expected = set()
          if wmi != "XXX":
            if year in ("M", "N", "P"):
              expected = {CAR.VOLKSWAGEN_ID4_MK1}
            elif chassis == "E8" and year in ("R", "S"):
              expected = {CAR.VOLKSWAGEN_ID4_MK2}
          for firmware in ({(0x757, None): [radar_fw]}, {}, {(0x757, None): [b"unknown"]}):
            with self.subTest(wmi=wmi, chassis=chassis, year=year, firmware=firmware):
              matches = FW_QUERY_CONFIG.match_fw_to_car_fuzzy(firmware, "".join(vin), FW_VERSIONS)
              self.assertEqual(matches, expected if firmware.get((0x757, None)) == [radar_fw] else set())

  def test_six_meb_interfaces_and_native_controller_frames(self):
    for car_model in self.MEB_BATCH:
      gen2 = bool(car_model.config.flags & VolkswagenFlags.MEB_GEN2)
      for gateway in (False, True):
        for alpha_long in (False, True):
          with self.subTest(car_model=car_model, gateway=gateway, alpha_long=alpha_long):
            fingerprint = {bus: {} for bus in range(8)}
            if gateway:
              fingerprint[1][0x13D] = 32  # observed QFK_01 at J533
            fingerprint[0][0x24C] = 16  # optional Side Assist
            cp = CarInterface.get_params(car_model, fingerprint, [], alpha_long, False, False)
            self.assertTrue(cp.flags & VolkswagenFlags.MEB)
            self.assertEqual(bool(cp.flags & VolkswagenFlags.MEB_GEN2), gen2)
            self.assertEqual(cp.safetyConfigs[-1].safetyModel, CarParams.SafetyModel.volkswagenMeb)
            self.assertEqual(cp.safetyConfigs[-1].safetyParam,
                             VolkswagenSafetyFlags.MEB_ALT_CRC if gen2 else 0)
            self.assertEqual(DBC[car_model][Bus.pt], "vw_meb_2024_generated" if gen2 else "vw_meb_generated")
            self.assertEqual(cp.dashcamOnly, not gateway)
            self.assertTrue(cp.openpilotLongitudinalControl)  # retained gateway system-long default
            self.assertFalse(cp.pcmCruise)
            if not gateway:
              # card.py makes dashcamOnly configurations passive with no-output safety.
              continue

            can = CanBus(cp)
            packer = CANPacker(DBC[car_model][Bus.pt])
            state = CarState(cp)
            parsers = state.get_can_parsers(cp)
            state.update(parsers)  # register lazily accessed DBC messages
            bsm_bus = can.pt if gen2 else can.cam
            bsm_parser = parsers[Bus.pt] if gen2 else parsers[Bus.cam]
            bsm = packer.make_can_msg("MEB_Side_Assist_01", bsm_bus, {"Blind_Spot_Info_Driver": 1})
            self.assertEqual(bsm_parser.update((1_000_000_000, [bsm])), {0x24C})
            incoming = [packer.make_can_msg("Motor_51", can.pt, {"TSK_Status": 3}),
                        packer.make_can_msg("Motor_54", can.pt, {"Engine_On": 1})]
            self.assertEqual(parsers[Bus.pt].update((1_000_000_000, incoming)), {0x10B, 0x14C})
            state.out = state.update(parsers)
            self.assertTrue(state.out.leftBlindspot)
            self.assertTrue(state.out.cruiseState.available)
            self.assertFalse(state.out.accFaulted)

            control = structs.CarControl()
            control.enabled = control.longActive = True
            control.actuators.accel = 1.0
            control.actuators.longControlState = structs.CarControl.Actuators.LongControlState.pid
            controller = CarController(DBC[car_model], cp)
            _, sent = controller.update(control.as_reader(), state, 1_000_000_000)
            self.assertEqual([(addr, len(data), bus) for addr, data, bus in sent],
                             [(0x303, 24, can.pt), (0x14D, 32, can.pt), (0x397, 8, can.pt), (0x300, 48, can.pt)])
            observed = CANParser(DBC[car_model][Bus.pt], [("HCA_03", 0), ("ACC_18", 0), ("ACC_19", 0)], can.pt)
            self.assertEqual(observed.update((1_010_000_000, sent)), {0x303, 0x14D, 0x300})
            self.assertEqual(observed.vl["ACC_18"]["ACC_Status_ACC"], 3)
            self.assertAlmostEqual(observed.vl["ACC_18"]["ACC_Sollbeschleunigung_02"], 1.0)

  def test_spare_part_fw_pattern(self):
    # Relied on for determining if a FW is likely VW
    for platform, ecus in FW_VERSIONS.items():
      with self.subTest(platform=platform.value):
        for fws in ecus.values():
          for fw in fws:
            assert SPARE_PART_FW_PATTERN.match(fw) is not None, f"Bad FW: {fw}"

  def test_chassis_codes(self):
    for platform in CAR:
      with self.subTest(platform=platform.value):
        assert len(platform.config.wmis) > 0, "WMIs not set"
        assert len(platform.config.chassis_codes) > 0, "Chassis codes not set"
        assert all(CHASSIS_CODE_PATTERN.match(cc) for cc in
                   platform.config.chassis_codes), "Bad chassis codes"

        # MEB model generations can share a chassis when VIN years separate them.
        for comp in CAR:
          if platform == comp:
            continue
          shared = platform.config.chassis_codes & comp.config.chassis_codes
          if shared:
            both_meb = bool(platform.config.flags & VolkswagenFlags.MEB and comp.config.flags & VolkswagenFlags.MEB)
            years_a = getattr(platform.config, "model_years", set())
            years_b = getattr(comp.config, "model_years", set())
            assert both_meb and years_a and years_b and not years_a & years_b, f"Shared chassis codes: {comp}"

  def test_custom_fuzzy_fingerprinting(self):
    all_radar_fw = list({fw for ecus in FW_VERSIONS.values() for fw in ecus[Ecu.fwdRadar, 0x757, None]})

    for platform in CAR:
      with self.subTest(platform=platform.name):
        for wmi in WMI:
          for chassis_code in platform.config.chassis_codes | {"00"}:
            for model_year in getattr(platform.config, "model_years", set()) or {"0"}:
              vin = ["0"] * 17
              vin[0:3] = wmi
              vin[6:8] = chassis_code
              vin[9] = model_year
              vin = "".join(vin)

              # VW radar part numbers are shared; VIN family/year resolves the car.
              for radar_fw in random.sample(all_radar_fw, 5) + [b'\xf1\x875Q0907572G \xf1\x890571', b'\xf1\x877H9907572AA\xf1\x890396']:
                should_match = ((wmi in platform.config.wmis and chassis_code in platform.config.chassis_codes) and
                                radar_fw in all_radar_fw)

                live_fws = {(0x757, None): [radar_fw]}
                matches = FW_QUERY_CONFIG.match_fw_to_car_fuzzy(live_fws, vin, FW_VERSIONS)

                expected_matches = {platform} if should_match else set()
                assert expected_matches == matches, "Bad match"


class TestVolkswagenMqbGasOverride(unittest.TestCase):
  @staticmethod
  def _acc_status_sequence(frames):
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], True, False, False)
    controller = CarController(DBC[CP.carFingerprint], CP)
    statuses = []
    for enabled, long_active, gas_pressed in frames:
      CC = structs.CarControl(enabled=enabled, longActive=long_active)
      controller.update_gas_override(SimpleNamespace(out=SimpleNamespace(gasPressed=gas_pressed)), CC)
      statuses.append(mqbcan.acc_control_value(True, False, CC.longActive, controller.gas_override))
    return statuses

  def test_gas_release_has_no_standby_frame(self):
    # longActive lags gasPressed by a frame on both press and release. A single ACC_STANDBY (2) frame between
    # ACC_OVERRIDE (4) and ACC_ACTIVE (3) makes TSK_06 drop to standby and then fault (routes 0000003c, 0000003d)
    frames = [
      (True, True, False),   # engaged
      (True, True, True),    # gas pressed, longActive not yet updated
      (True, False, True),   # override
      (True, False, True),
      (True, False, False),  # gas released, longActive not yet updated
      (True, True, False),   # engaged again
    ]
    self.assertEqual(self._acc_status_sequence(frames), [3, 3, 4, 4, 4, 3])

  def test_override_requires_gas(self):
    # Long paused for reasons other than the gas pedal (e.g. pauseLongitudinal) is not an override
    self.assertEqual(self._acc_status_sequence([(True, True, False), (True, False, False), (True, True, False)]), [3, 2, 3])

  def test_override_ends_on_disengage(self):
    self.assertEqual(self._acc_status_sequence([(True, False, True), (False, False, False), (False, False, True)]), [4, 2, 2])


class TestVolkswagenHcaAutoStop(unittest.TestCase):
  def _car_state(self):
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], False, False, False)
    CS = CarState(CP)
    CS.eps_init_complete = True
    return CS

  def _perm_fault(self, CS, hca_status, engine_autostop=False):
    CS.frame += 1
    return CS.update_hca_state(hca_status, engine_autostop=engine_autostop)[1]

  def test_fault_is_permanent_without_autostop(self):
    CS = self._car_state()
    self.assertTrue(self._perm_fault(CS, "FAULT"))

  def test_fault_ignored_around_autostop(self):
    CS = self._car_state()
    self.assertFalse(self._perm_fault(CS, "FAULT", engine_autostop=True))
    for _ in range(HCA_AUTOSTOP_GRACE_FRAMES - 1):
      self.assertFalse(self._perm_fault(CS, "FAULT"))
    # The grace window ends a fixed time after the engine restarts
    self.assertTrue(self._perm_fault(CS, "FAULT"))


class TestVolkswagenMqbMainSwitchCancel(unittest.TestCase):
  @staticmethod
  def _cancel_events(frames, alpha_long=True, push_button=True):
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], alpha_long, False, False)
    buttons = CCP(CP).BUTTONS
    CS = CarState.__new__(CarState)
    CS.button_states = {button.event_type: False for button in buttons}
    released = ("GRA_Tip_Setzen", "GRA_Tip_Wiederaufnahme", "GRA_Tip_Hoch", "GRA_Tip_Runter", "GRA_Verstellung_Zeitluecke")
    events = []
    for abbrechen, hauptschalter in frames:
      gra = dict.fromkeys(released, 0) | {"GRA_Abbrechen": abbrechen, "GRA_Hauptschalter": hauptschalter,
                                              "GRA_Typ_Hauptschalter": int(push_button)}
      button_events = CS.create_button_events(SimpleNamespace(vl={"GRA_ACC_01": gra}), buttons)
      events.append([be.pressed for be in button_events if be.type == structs.CarState.ButtonEvent.Type.cancel])
    return events

  def test_main_switch_cancels_under_openpilot_long(self):
    # Some MFL wheels have no cancel button, and nothing else acts on O/I under openpilot long
    self.assertEqual(self._cancel_events([(0, 0), (0, 1), (0, 1), (0, 0)]), [[], [True], [], [False]])

  def test_overlapping_cancel_signals_are_one_press(self):
    self.assertEqual(self._cancel_events([(1, 0), (1, 1), (0, 1), (0, 0)]), [[True], [], [], [False]])

  def test_latched_main_switch_is_not_cancel(self):
    # A latched stalk switch holds GRA_Hauptschalter at 1 while on; the stalk's own cancel must still edge
    self.assertEqual(self._cancel_events([(0, 1), (1, 1), (0, 1), (0, 0)], push_button=False), [[], [True], [False], []])

  def test_main_switch_untouched_with_stock_long(self):
    self.assertEqual(self._cancel_events([(0, 0), (0, 1), (0, 0)], alpha_long=False), [[], [], []])


class TestVolkswagenMqbStopping(unittest.TestCase):
  @staticmethod
  def _acc_07(stopping, starting, esp_hold, **kwargs):
    packer = CANPacker(DBC[CAR.VOLKSWAGEN_GOLF_MK7][Bus.pt])
    msgs = mqbcan.create_acc_accel_control(packer, 0, 0, True, -0.5, 3, stopping, starting, esp_hold, **kwargs)
    dat = int.from_bytes(bytes(next(m[1] for m in msgs if m[0] == 0x12E)), 'little')  # ACC_07
    sigs = DBCParsed('vw_mqb').name_to_msg["ACC_07"].sigs

    def value(name):
      sig = sigs[name]
      return ((dat >> sig.start_bit) & ((1 << sig.size) - 1)) * sig.factor + sig.offset
    return value

  def test_distance_matches_requested_decel(self):
    # Stock radar: 0.82 m at 3.4 km/h with a -0.54 m/s^2 request
    self.assertAlmostEqual(mqbcan.acc_stopping_distance(3.4 / 3.6, -0.54), 0.82, delta=0.05)
    # Entering stopping while still rolling no longer asks the ESP to stop within 0.3 m; like the stock radar, the
    # distance never implies more than ~1 m/s^2
    v = 5.9 / 3.6
    self.assertAlmostEqual(v ** 2 / (2 * mqbcan.acc_stopping_distance(v, -1.17)), mqbcan.ACC_STOPPING_DECEL_MAX, delta=0.01)

  def test_distance_limits(self):
    self.assertEqual(mqbcan.acc_stopping_distance(0., -0.55), mqbcan.ACC_STOPPING_DISTANCE_MIN)
    self.assertEqual(mqbcan.acc_stopping_distance(30., -0.5), mqbcan.ACC_STOPPING_DISTANCE_MAX)
    # A near-zero or positive request still assumes a gentle stop rather than an infinite distance
    self.assertEqual(mqbcan.acc_stopping_distance(1.6, 0.2), mqbcan.acc_stopping_distance(1.6, -mqbcan.ACC_STOPPING_DECEL_MIN))

  def test_distance_sent_only_while_stopping(self):
    self.assertAlmostEqual(self._acc_07(True, False, False, stopping_distance=1.2)("ACC_Anhalteweg"), 1.2, delta=0.02)
    self.assertAlmostEqual(self._acc_07(False, False, False, stopping_distance=1.2)("ACC_Anhalteweg"),
                           mqbcan.ACC_STOPPING_DISTANCE_MAX, delta=0.02)

  def test_hold_sequence_matches_stock(self):
    # Stock radar: standby (3) while the ESP brings the car to a stop, hold request (1) once it confirms the hold,
    # release (4) on drive off
    self.assertEqual(self._acc_07(stopping=True, starting=False, esp_hold=False)("ACC_Anforderung_HMS"), 3)
    self.assertEqual(self._acc_07(stopping=True, starting=False, esp_hold=True)("ACC_Anforderung_HMS"), 1)
    self.assertEqual(self._acc_07(stopping=False, starting=True, esp_hold=True)("ACC_Anforderung_HMS"), 4)
    self.assertEqual(self._acc_07(stopping=False, starting=False, esp_hold=False)("ACC_Anforderung_HMS"), 0)


class TestVolkswagenMqbLeadIcon(unittest.TestCase):
  def test_position_scale(self):
    # At the radar's reference gap (~4.2 m + 0.9 s) the lead reads ~101; far reads at the radar's maximum
    assert abs(mqbcan.lead_icon_position(4.16 + 0.90 * 25., 25.) - 118) < 1
    assert mqbcan.lead_icon_position(150., 25.) == 972
    # Stopped behind a car is never shown as inside the gap; at speed it can be
    assert mqbcan.lead_icon_position(2., 0.) == 101
    assert mqbcan.lead_icon_position(10., 30.) == 34

  def test_position_monotonic(self):
    # Further away reads further; the same gap at a higher speed reads closer, like the stock radar
    positions = [mqbcan.lead_icon_position(d, 25.) for d in range(5, 150, 5)]
    assert positions == sorted(positions)
    assert mqbcan.lead_icon_position(40., 30.) < mqbcan.lead_icon_position(40., 15.)

  def test_icon_snaps_to_notches_and_glides(self):
    icon = mqbcan.LeadIcon(0.06)
    first = icon.update(40., 25.)
    assert first in mqbcan.LEAD_ICON_NOTCHES  # a new lead is placed directly on a notch
    # Small changes in distance don't move the icon off its notch
    for d in (39., 41., 40.5, 39.5):
      assert icon.update(d, 25.) == first
    # A clearly further lead moves to a further notch, easing there over a few seconds rather than jumping
    steps = [icon.update(60., 25.) for _ in range(150)]
    assert steps[-1] > first
    assert steps == sorted(steps)
    assert max(np.diff([first] + steps)) < 67 / 2  # glides, never jumps a whole notch
    assert steps[-1] in mqbcan.LEAD_ICON_NOTCHES
    # Losing the lead clears the icon
    assert icon.update(0., 25.) == 0

  @staticmethod
  def _sent_lead_distance(lead_visible, lead_distance, enabled=True, upscale=True):
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], True, False, False)
    controller = CarController(DBC[CP.carFingerprint], CP)
    controller.frame = controller.CCP.ACC_HUD_STEP * 200  # a frame that sends ACC_02, after the 1 s display delay
    CS = SimpleNamespace(out=SimpleNamespace(gasPressed=False, standstill=False, cruiseState=SimpleNamespace(available=True),
                                             accFaulted=False, steeringPressed=False, steeringTorque=0.0, vEgo=10.0, vEgoRaw=10.0),
                         upscale_lead_car_signal=upscale, ldw_stock_values={}, gra_stock_values={"COUNTER": 0},
                         acc_type=0, esp_hold_confirmation=False, eps_stock_values={}, stock_acc_lead_index=0)
    CC = structs.CarControl(enabled=enabled, longActive=enabled)
    CC.hudControl.leadVisible = lead_visible
    CC.hudControl.leadDistance = lead_distance
    sent = [d for addr, d, bus in controller.update(CC.as_reader(), CS, 0)[1] if addr == 0x30C]
    return (int.from_bytes(bytes(sent[0]), 'little') >> 24) & 0x3FF

  def test_hud_uses_openpilot_lead(self):
    assert self._sent_lead_distance(True, 25.0) == mqbcan.LeadIcon(0.06).update(25.0, 10.0)
    assert self._sent_lead_distance(True, 0.0) == 512  # no lead distance: fixed position as before
    assert self._sent_lead_distance(False, 25.0) == 0
    assert self._sent_lead_distance(True, 25.0, upscale=False) == 8  # analogue cluster scale unknown: unchanged

  def test_hud_lead_same_when_not_engaged(self):
    # The icon follows the lead the same way in standby as when engaged (route 00000042 sat at 512 in standby)
    assert self._sent_lead_distance(True, 25.0, enabled=False) == self._sent_lead_distance(True, 25.0, enabled=True)
