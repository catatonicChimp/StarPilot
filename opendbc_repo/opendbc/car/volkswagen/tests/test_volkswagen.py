import random
import re
from types import SimpleNamespace

import numpy as np
import pytest

from opendbc.can.dbc import DBC as DBCParsed
from opendbc.can.packer import CANPacker
from opendbc.car import Bus
from opendbc.car.structs import CarControl, CarParams
from opendbc.car.volkswagen import mqbcan
from opendbc.car.volkswagen.carcontroller import CarController
from opendbc.car.volkswagen.interface import CarInterface
from opendbc.car.volkswagen.fingerprints import FW_VERSIONS
from opendbc.car.volkswagen.mqbcan import volkswagen_meb_alt_crc_checksum, volkswagen_mqb_meb_checksum
from opendbc.car.volkswagen.radar_interface import RadarInterface
from opendbc.car.volkswagen.values import CAR, DBC, FW_QUERY_CONFIG, WMI, CanBus, VolkswagenFlags, VolkswagenSafetyFlags

Ecu = CarParams.Ecu

CHASSIS_CODE_PATTERN = re.compile('[A-Z0-9]{2}')
# TODO: determine the unknown groups
SPARE_PART_FW_PATTERN = re.compile(b'\xf1\x87(?P<gateway>[0-9][0-9A-Z]{2})(?P<unknown>[0-9][0-9A-Z][0-9])(?P<unknown2>[0-9A-Z]{2}[0-9])([A-Z0-9]| )')


class TestVolkswagenPlatformConfigs:
  MEB_CARS = {car for car in CAR if car.config.flags & VolkswagenFlags.MEB}

  @staticmethod
  def _get_meb_params(car, gateway=True, alpha_long=False):
    fingerprint = {bus: {} for bus in range(8)}
    if gateway:
      fingerprint[1][0x13D] = 32
    return CarInterface.get_params(car, fingerprint, [], alpha_long, False, False, None)

  def test_meb_platform_params(self):
    for car in self.MEB_CARS:
      cp = self._get_meb_params(car)
      assert cp.flags & VolkswagenFlags.MEB
      assert cp.transmissionType == CarParams.TransmissionType.direct
      assert cp.steerControlType == CarParams.SteerControlType.curvatureDEPRECATED
      assert cp.steerAtStandstill
      assert cp.safetyConfigs[-1].safetyModel == CarParams.SafetyModel.volkswagenMeb
      assert not cp.dashcamOnly
      assert not cp.radarUnavailable

      has_gen2_crc = bool(cp.safetyConfigs[-1].safetyParam & VolkswagenSafetyFlags.MEB_ALT_CRC)
      assert has_gen2_crc == bool(car.config.flags & VolkswagenFlags.MEB_GEN2)

  def test_meb_camera_harness_is_passive(self):
    cp = self._get_meb_params(CAR.VOLKSWAGEN_ID4_MK1, gateway=False, alpha_long=True)
    assert cp.dashcamOnly
    assert cp.radarUnavailable
    assert not cp.alphaLongitudinalAvailable
    assert not cp.openpilotLongitudinalControl
    assert not (cp.safetyConfigs[-1].safetyParam & VolkswagenSafetyFlags.LONG_CONTROL)

  def test_meb_docs_assume_required_gateway_harness(self):
    fingerprint = {bus: {} for bus in range(8)}
    cp = CarInterface.get_params(CAR.VOLKSWAGEN_ID4_MK1, fingerprint, [], True, False, True, None)

    assert cp.networkLocation == CarParams.NetworkLocation.gateway
    assert not cp.dashcamOnly
    assert cp.alphaLongitudinalAvailable

  def test_meb_gateway_longitudinal(self):
    cp = self._get_meb_params(CAR.VOLKSWAGEN_ID4_MK1, gateway=True, alpha_long=True)
    assert cp.alphaLongitudinalAvailable
    assert cp.openpilotLongitudinalControl
    assert not cp.pcmCruise
    assert cp.safetyConfigs[-1].safetyParam & VolkswagenSafetyFlags.LONG_CONTROL

  @pytest.mark.parametrize("data_hex", (
    "fc03fcfcfc0f0000",
    "e304fcfcfc0f0000",
    "1105fcfcfc0f0000",
  ))
  def test_meb_klr_checksum(self, data_hex):
    data = bytearray.fromhex(data_hex)
    assert volkswagen_mqb_meb_checksum(0x25D, None, data) == data[0]

  @pytest.mark.parametrize(("address", "data_hex"), (
    (0x0DB, "bb0ffcf0fefe0000fd0fffc0ff0000000200000000000000010000000000000000000000000000000000000000000000"),
    (0x0FC, "650b1f007ef0b10c0000000000000000ffff1019191c1cfefe0000000000000000e0fff40140ffeb7f0748e481af421f00000000000000000000000000000000"),
    (0x102, "9f0e7cfa010500000020cb0402000000b703a00000ec0f00000000002cd3ff1f0020a60000000020000000007d5256ab"),
    (0x10B, "9d06000000007efe000000010000ff01feff000000000000000000000090240000000000000000000000000000000000"),
    (0x139, "ac0e850b0890132000d019800000000000000000000000003002000500000000"),
    (0x13D, "2412111101d1060000d0d410d106000000000000000000000000000000000000"),
  ))
  def test_meb_gen2_checksum(self, address, data_hex):
    data = bytearray.fromhex(data_hex)
    assert volkswagen_meb_alt_crc_checksum(address, None, data) == data[0]

  def test_meb_camera_radar_tracks(self):
    cp = self._get_meb_params(CAR.SKODA_ENYAQ_MK1)
    radar = RadarInterface(cp)
    packer = CANPacker(DBC[cp.carFingerprint][Bus.radar])
    message = packer.make_can_msg("MEB_Distance_01", CanBus(cp).cam, {
      "Distance_Status": 0,
      "Same_Lane_01_ObjectID": 1,
      "Same_Lane_01_Long_Distance": 25.0,
      "Same_Lane_01_Lat_Distance": 0.5,
      "Same_Lane_01_Rel_Velo": -2.0,
    })

    radar_data = radar.update([(1_000_000_000, [message])])
    assert radar_data is not None
    assert len(radar_data.points) == 1
    assert radar_data.points[0].trackId == 0
    assert radar_data.points[0].dRel == pytest.approx(25.0, abs=0.1)
    assert radar_data.points[0].yRel == pytest.approx(0.5, abs=0.1)
    assert radar_data.points[0].vRel == pytest.approx(-2.0, abs=0.1)

  def test_taos_longitudinal_actuator_delay(self):
    taos_cp = CarInterface.get_non_essential_params(CAR.VOLKSWAGEN_TAOS_MK1)
    golf_cp = CarInterface.get_non_essential_params(CAR.VOLKSWAGEN_GOLF_MK7)

    assert abs(taos_cp.longitudinalActuatorDelay - 0.25) < 1e-6
    assert abs(golf_cp.longitudinalActuatorDelay - 0.15) < 1e-6

  def test_spare_part_fw_pattern(self, subtests):
    # Relied on for determining if a FW is likely VW
    for platform, ecus in FW_VERSIONS.items():
      with subtests.test(platform=platform.value):
        for fws in ecus.values():
          for fw in fws:
            assert SPARE_PART_FW_PATTERN.match(fw) is not None, f"Bad FW: {fw}"

  def test_chassis_codes(self, subtests):
    for platform in CAR:
      with subtests.test(platform=platform.value):
        assert len(platform.config.wmis) > 0, "WMIs not set"
        assert len(platform.config.chassis_codes) > 0, "Chassis codes not set"
        assert all(CHASSIS_CODE_PATTERN.match(cc) for cc in
                   platform.config.chassis_codes), "Bad chassis codes"

        # Shared MEB chassis codes are valid only when VIN model-year sets are disjoint.
        for comp in CAR:
          if platform == comp:
            continue
          shared_chassis = platform.config.chassis_codes & comp.config.chassis_codes
          if shared_chassis:
            both_meb = platform.config.flags & VolkswagenFlags.MEB and comp.config.flags & VolkswagenFlags.MEB
            disjoint_years = (getattr(platform.config, "model_years", set()) and getattr(comp.config, "model_years", set()) and
                              not platform.config.model_years & comp.config.model_years)
            assert both_meb and disjoint_years, f"Shared chassis codes: {comp}"

  def test_custom_fuzzy_fingerprinting(self, subtests):
    all_radar_fw = list({fw for ecus in FW_VERSIONS.values() for fw in ecus[Ecu.fwdRadar, 0x757, None]})

    for platform in CAR:
      with subtests.test(platform=platform.name):
        model_years = getattr(platform.config, "model_years", set()) or {"0"}
        for wmi in WMI:
          for chassis_code in platform.config.chassis_codes | {"00"}:
            for model_year in model_years:
              vin = ["0"] * 17
              vin[0:3] = wmi
              vin[6:8] = chassis_code
              vin[9] = model_year
              vin = "".join(vin)

              # Check a few FW cases - expected, unexpected
              for radar_fw in random.sample(all_radar_fw, 5) + [b'\xf1\x875Q0907572G \xf1\x890571', b'\xf1\x877H9907572AA\xf1\x890396']:
                should_match = ((wmi in platform.config.wmis and chassis_code in platform.config.chassis_codes) and
                                radar_fw in all_radar_fw)

                live_fws = {(0x757, None): [radar_fw]}
                matches = FW_QUERY_CONFIG.match_fw_to_car_fuzzy(live_fws, vin, FW_VERSIONS)

                expected_matches = {platform} if should_match else set()
                assert expected_matches == matches, "Bad match"


class TestVolkswagenMqbGasOverride:
  @staticmethod
  def _acc_status_sequence(frames):
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], True, False, False, None)
    controller = CarController(DBC[CP.carFingerprint], CP)
    statuses = []
    for enabled, long_active, gas_pressed in frames:
      CC = CarControl(enabled=enabled, longActive=long_active)
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
    assert self._acc_status_sequence(frames) == [3, 3, 4, 4, 4, 3]

  def test_override_requires_gas(self):
    # Long paused for reasons other than the gas pedal (e.g. pauseLongitudinal) is not an override
    assert self._acc_status_sequence([(True, True, False), (True, False, False), (True, True, False)]) == [3, 2, 3]

  def test_override_ends_on_disengage(self):
    assert self._acc_status_sequence([(True, False, True), (False, False, False), (False, False, True)]) == [4, 2, 2]


class TestVolkswagenMqbLeadIcon:
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
    CP = CarInterface.get_params(CAR.VOLKSWAGEN_GOLF_MK7, {bus: {} for bus in range(8)}, [], True, False, False, None)
    controller = CarController(DBC[CP.carFingerprint], CP)
    controller.frame = controller.CCP.ACC_HUD_STEP * 200  # a frame that sends ACC_02, after the 1 s display delay
    CS = SimpleNamespace(out=SimpleNamespace(gasPressed=False, standstill=False, cruiseState=SimpleNamespace(available=True),
                                             accFaulted=False, steeringPressed=False, vEgo=10.0, vEgoRaw=10.0),
                         upscale_lead_car_signal=upscale, ldw_stock_values={}, gra_stock_values={"COUNTER": 0},
                         acc_type=0, esp_hold_confirmation=False, eps_stock_values={})
    CC = CarControl(enabled=enabled, longActive=enabled)
    CC.hudControl.leadVisible = lead_visible
    CC.hudControl.leadDistance = lead_distance
    sent = [d for addr, d, bus in controller.update(CC.as_reader(), CS, 0, SimpleNamespace(vEgoStopping=0.5))[1] if addr == 0x30C]
    return (int.from_bytes(bytes(sent[0]), 'little') >> 24) & 0x3FF

  def test_hud_uses_openpilot_lead(self):
    assert self._sent_lead_distance(True, 25.0) == mqbcan.LeadIcon(0.06).update(25.0, 10.0)
    assert self._sent_lead_distance(True, 0.0) == 512  # no lead distance: fixed position as before
    assert self._sent_lead_distance(False, 25.0) == 0
    assert self._sent_lead_distance(True, 25.0, upscale=False) == 8  # analogue cluster scale unknown: unchanged

  def test_hud_lead_same_when_not_engaged(self):
    # The icon follows the lead the same way in standby as when engaged (route 00000042 sat at 512 in standby)
    assert self._sent_lead_distance(True, 25.0, enabled=False) == self._sent_lead_distance(True, 25.0, enabled=True)


DBC_PARSED = DBCParsed('vw_mqb')


class TestVolkswagenMqbStoppingDistance:
  def test_matches_requested_decel(self):
    # Stock radar on route 00000038: 0.82 m at 3.4 km/h with a -0.54 m/s^2 request
    assert abs(mqbcan.acc_stopping_distance(3.4 / 3.6, -0.54) - 0.82) < 0.05
    # Entering stopping while still rolling no longer asks the ESP to stop within 0.3 m (route 00000045: -3.3 m/s^2);
    # like the stock radar, the distance never implies more than ~1 m/s^2
    v = 5.9 / 3.6
    distance = mqbcan.acc_stopping_distance(v, -1.17)
    assert abs(v ** 2 / (2 * distance) - mqbcan.ACC_STOPPING_DECEL_MAX) < 0.01

  def test_limits(self):
    assert mqbcan.acc_stopping_distance(0., -0.55) == mqbcan.ACC_STOPPING_DISTANCE_MIN
    assert mqbcan.acc_stopping_distance(30., -0.5) == mqbcan.ACC_STOPPING_DISTANCE_MAX
    # A near-zero or positive request still assumes a gentle stop rather than an infinite distance
    assert mqbcan.acc_stopping_distance(1.6, 0.2) == mqbcan.acc_stopping_distance(1.6, -mqbcan.ACC_STOPPING_DECEL_MIN)

  def test_sent_only_while_stopping(self):
    packer = CANPacker(DBC[CAR.VOLKSWAGEN_GOLF_MK7][Bus.pt])

    def sent_distance(stopping):
      msgs = mqbcan.create_acc_accel_control(packer, 0, 0, True, -1.0, 3, stopping, False, False, stopping_distance=1.2)
      dat = next(m[1] for m in msgs if m[0] == 0x12E)  # ACC_07
      sig = DBC_PARSED.name_to_msg["ACC_07"].sigs["ACC_Anhalteweg"]
      raw = (int.from_bytes(bytes(dat), 'little') >> sig.start_bit) & ((1 << sig.size) - 1)
      return raw * sig.factor + sig.offset
    assert abs(sent_distance(True) - 1.2) < 0.02
    assert abs(sent_distance(False) - mqbcan.ACC_STOPPING_DISTANCE_MAX) < 0.02

  def test_hold_sequence_matches_stock(self):
    # Stock radar: standby (3) while the ESP brings the car to a stop, hold request (1) once it confirms the hold,
    # release (4) on drive off
    packer = CANPacker(DBC[CAR.VOLKSWAGEN_GOLF_MK7][Bus.pt])
    sig = DBC_PARSED.name_to_msg["ACC_07"].sigs["ACC_Anforderung_HMS"]

    def hold_type(stopping, starting, esp_hold):
      msgs = mqbcan.create_acc_accel_control(packer, 0, 0, True, -0.5, 3, stopping, starting, esp_hold)
      dat = next(m[1] for m in msgs if m[0] == 0x12E)
      return (int.from_bytes(bytes(dat), 'little') >> sig.start_bit) & ((1 << sig.size) - 1)
    assert hold_type(stopping=True, starting=False, esp_hold=False) == 3
    assert hold_type(stopping=True, starting=False, esp_hold=True) == 1
    assert hold_type(stopping=False, starting=True, esp_hold=True) == 4
    assert hold_type(stopping=False, starting=False, esp_hold=False) == 0


class TestVolkswagenMqbSetSpeedMarker:
  def hud(self, status, reached=False):
    packer = CANPacker(DBC[CAR.VOLKSWAGEN_GOLF_MK7][Bus.pt])
    dat = int.from_bytes(bytes(mqbcan.create_acc_hud_control(packer, 0, status, 50., 0, 1, speed_reached=reached)[1]), 'little')
    get = lambda name: (dat >> DBC_PARSED.name_to_msg["ACC_02"].sigs[name].start_bit) & 1
    return get("ACC_Tachokranz"), get("ACC_Wunschgeschw_erreicht")

  def test_marker_on_while_engaged(self):
    # Stock radar keeps the speedometer set-speed marker on while engaged (3) or overridden (4), off otherwise
    assert self.hud(3)[0] == 1 and self.hud(4)[0] == 1
    assert self.hud(2)[0] == 0 and self.hud(0)[0] == 0 and self.hud(6)[0] == 0
    assert self.hud(3, reached=True)[1] == 1 and self.hud(3)[1] == 0

  def test_reached_matches_stock(self):
    # (v_ego, set speed, stock ACC_Wunschgeschw_erreicht) from stock ACC on route 00000038, in order. Stock also waits
    # ~2 s after engaging or a set speed change before setting it, which isn't modelled
    trace = [(39.7, 40, 1), (39.5, 41.92, 1), (39.3, 41.92, 1), (34.6, 41.92, 0),
             (47.1, 40, 0), (45.8, 40, 0), (44.6, 49.92, 0), (37.4, 40, 1)]
    reached = False
    for v, set_speed, stock in trace[:4]:
      reached = mqbcan.set_speed_reached(reached, True, v, set_speed)
      assert reached == bool(stock), (v, set_speed)
    reached = False
    for v, set_speed, stock in trace[4:7]:
      reached = mqbcan.set_speed_reached(reached, True, v, set_speed)
      assert reached == bool(stock), (v, set_speed)
    assert mqbcan.set_speed_reached(False, True, 37.4, 40)
    assert not mqbcan.set_speed_reached(True, False, 40, 40)
