"""lead_distance_ratio reads a real radarState lead (display only, VW MQB cluster lead icon)."""

import unittest

import numpy as np

import openpilot.cereal.messaging as messaging
from openpilot.selfdrive.controls.lib.longitudinal_planner import lead_distance_ratio


class _Mpc:
  def __init__(self, t_follow):
    self.params = np.zeros((1, 6))
    self.params[0, 4] = t_follow


class TestLeadDistanceRatio(unittest.TestCase):
  def _lead(self, present, d_rel=30.0, v_lead=20.0):
    msg = messaging.new_message('radarState')
    lead = msg.radarState.leadOne
    lead.present = present
    lead.dRel = d_rel
    lead.vLead = v_lead
    return msg.radarState.leadOne

  def test_no_lead_is_zero(self):
    self.assertEqual(lead_distance_ratio(self._lead(False), 20.0, _Mpc(1.45)), 0.0)

  def test_present_lead_gives_ratio(self):
    self.assertGreater(lead_distance_ratio(self._lead(True), 20.0, _Mpc(1.45)), 0.0)

  def test_bad_follow_time_is_zero(self):
    self.assertEqual(lead_distance_ratio(self._lead(True), 20.0, _Mpc(0.0)), 0.0)


if __name__ == '__main__':
  unittest.main()
