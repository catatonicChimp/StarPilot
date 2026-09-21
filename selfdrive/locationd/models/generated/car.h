#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_2515398924308937197);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5944693023187542615);
void car_H_mod_fun(double *state, double *out_5910940767233550556);
void car_f_fun(double *state, double dt, double *out_1593669514807352017);
void car_F_fun(double *state, double dt, double *out_4859662463778300068);
void car_h_25(double *state, double *unused, double *out_3109249876623915000);
void car_H_25(double *state, double *unused, double *out_2387960117244058879);
void car_h_24(double *state, double *unused, double *out_8243310450403332252);
void car_H_24(double *state, double *unused, double *out_3423425513778718086);
void car_h_30(double *state, double *unused, double *out_7444554702201113257);
void car_H_30(double *state, double *unused, double *out_4528730224247557876);
void car_h_26(double *state, double *unused, double *out_8189637268238584976);
void car_H_26(double *state, double *unused, double *out_6129463436118115103);
void car_h_27(double *state, double *unused, double *out_1758996010113418958);
void car_H_27(double *state, double *unused, double *out_4692062376187723860);
void car_h_29(double *state, double *unused, double *out_1878453518140603392);
void car_H_29(double *state, double *unused, double *out_640604185577581932);
void car_h_28(double *state, double *unused, double *out_7089545780969910120);
void car_H_28(double *state, double *unused, double *out_4441794831491948642);
void car_h_31(double *state, double *unused, double *out_5310037885145381436);
void car_H_31(double *state, double *unused, double *out_2357314155367098451);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}