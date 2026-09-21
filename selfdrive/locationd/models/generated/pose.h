#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6754834019914573692);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1942039733683991315);
void pose_H_mod_fun(double *state, double *out_7857849580727627842);
void pose_f_fun(double *state, double dt, double *out_5326446350083772854);
void pose_F_fun(double *state, double dt, double *out_5404773275233196712);
void pose_h_4(double *state, double *unused, double *out_5757806951307091776);
void pose_H_4(double *state, double *unused, double *out_1086512085330193500);
void pose_h_10(double *state, double *unused, double *out_6766273307206545632);
void pose_H_10(double *state, double *unused, double *out_5690926367066679972);
void pose_h_13(double *state, double *unused, double *out_5663744141387295532);
void pose_H_13(double *state, double *unused, double *out_4920267548632717524);
void pose_h_14(double *state, double *unused, double *out_2121763880548370869);
void pose_H_14(double *state, double *unused, double *out_4169300517625565796);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}