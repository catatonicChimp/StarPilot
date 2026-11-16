"""Optional VW MQB gateway stock-radar observation for logging; never a longitudinal control grant."""
from opendbc.car.volkswagen.values import NetworkLocation, VolkswagenFlags
from openpilot.cereal import messaging

NON_MQB_FLAGS = VolkswagenFlags.PQ | VolkswagenFlags.MEB | VolkswagenFlags.MLB


def capable(cp):
  return (cp.brand == 'volkswagen' and not cp.notCar and not int(cp.flags) & NON_MQB_FLAGS and
          cp.networkLocation == NetworkLocation.gateway)


def compose(cp, parsed, state, message, *, car_state_stamp):
  if not capable(cp):
    return message
  lead_index = getattr(parsed, 'stock_acc_lead_index', None)
  follow_accel = getattr(parsed, 'stock_acc_follow_accel', None)
  if lead_index is None or follow_accel is None or not state.canValid or state.canTimeout:
    return message
  if message is None:
    message = messaging.new_message('starpilotCarState', valid=bool(state.canValid), logMonoTime=car_state_stamp)
    message.starpilotCarState.sourceCarStateMonoTime = car_state_stamp
  message.starpilotCarState.stockAccLeadIndex = int(lead_index)
  message.starpilotCarState.stockAccFollowAccel = float(follow_accel)
  return message
