#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2515398924308937197) {
   out_2515398924308937197[0] = delta_x[0] + nom_x[0];
   out_2515398924308937197[1] = delta_x[1] + nom_x[1];
   out_2515398924308937197[2] = delta_x[2] + nom_x[2];
   out_2515398924308937197[3] = delta_x[3] + nom_x[3];
   out_2515398924308937197[4] = delta_x[4] + nom_x[4];
   out_2515398924308937197[5] = delta_x[5] + nom_x[5];
   out_2515398924308937197[6] = delta_x[6] + nom_x[6];
   out_2515398924308937197[7] = delta_x[7] + nom_x[7];
   out_2515398924308937197[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5944693023187542615) {
   out_5944693023187542615[0] = -nom_x[0] + true_x[0];
   out_5944693023187542615[1] = -nom_x[1] + true_x[1];
   out_5944693023187542615[2] = -nom_x[2] + true_x[2];
   out_5944693023187542615[3] = -nom_x[3] + true_x[3];
   out_5944693023187542615[4] = -nom_x[4] + true_x[4];
   out_5944693023187542615[5] = -nom_x[5] + true_x[5];
   out_5944693023187542615[6] = -nom_x[6] + true_x[6];
   out_5944693023187542615[7] = -nom_x[7] + true_x[7];
   out_5944693023187542615[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_5910940767233550556) {
   out_5910940767233550556[0] = 1.0;
   out_5910940767233550556[1] = 0.0;
   out_5910940767233550556[2] = 0.0;
   out_5910940767233550556[3] = 0.0;
   out_5910940767233550556[4] = 0.0;
   out_5910940767233550556[5] = 0.0;
   out_5910940767233550556[6] = 0.0;
   out_5910940767233550556[7] = 0.0;
   out_5910940767233550556[8] = 0.0;
   out_5910940767233550556[9] = 0.0;
   out_5910940767233550556[10] = 1.0;
   out_5910940767233550556[11] = 0.0;
   out_5910940767233550556[12] = 0.0;
   out_5910940767233550556[13] = 0.0;
   out_5910940767233550556[14] = 0.0;
   out_5910940767233550556[15] = 0.0;
   out_5910940767233550556[16] = 0.0;
   out_5910940767233550556[17] = 0.0;
   out_5910940767233550556[18] = 0.0;
   out_5910940767233550556[19] = 0.0;
   out_5910940767233550556[20] = 1.0;
   out_5910940767233550556[21] = 0.0;
   out_5910940767233550556[22] = 0.0;
   out_5910940767233550556[23] = 0.0;
   out_5910940767233550556[24] = 0.0;
   out_5910940767233550556[25] = 0.0;
   out_5910940767233550556[26] = 0.0;
   out_5910940767233550556[27] = 0.0;
   out_5910940767233550556[28] = 0.0;
   out_5910940767233550556[29] = 0.0;
   out_5910940767233550556[30] = 1.0;
   out_5910940767233550556[31] = 0.0;
   out_5910940767233550556[32] = 0.0;
   out_5910940767233550556[33] = 0.0;
   out_5910940767233550556[34] = 0.0;
   out_5910940767233550556[35] = 0.0;
   out_5910940767233550556[36] = 0.0;
   out_5910940767233550556[37] = 0.0;
   out_5910940767233550556[38] = 0.0;
   out_5910940767233550556[39] = 0.0;
   out_5910940767233550556[40] = 1.0;
   out_5910940767233550556[41] = 0.0;
   out_5910940767233550556[42] = 0.0;
   out_5910940767233550556[43] = 0.0;
   out_5910940767233550556[44] = 0.0;
   out_5910940767233550556[45] = 0.0;
   out_5910940767233550556[46] = 0.0;
   out_5910940767233550556[47] = 0.0;
   out_5910940767233550556[48] = 0.0;
   out_5910940767233550556[49] = 0.0;
   out_5910940767233550556[50] = 1.0;
   out_5910940767233550556[51] = 0.0;
   out_5910940767233550556[52] = 0.0;
   out_5910940767233550556[53] = 0.0;
   out_5910940767233550556[54] = 0.0;
   out_5910940767233550556[55] = 0.0;
   out_5910940767233550556[56] = 0.0;
   out_5910940767233550556[57] = 0.0;
   out_5910940767233550556[58] = 0.0;
   out_5910940767233550556[59] = 0.0;
   out_5910940767233550556[60] = 1.0;
   out_5910940767233550556[61] = 0.0;
   out_5910940767233550556[62] = 0.0;
   out_5910940767233550556[63] = 0.0;
   out_5910940767233550556[64] = 0.0;
   out_5910940767233550556[65] = 0.0;
   out_5910940767233550556[66] = 0.0;
   out_5910940767233550556[67] = 0.0;
   out_5910940767233550556[68] = 0.0;
   out_5910940767233550556[69] = 0.0;
   out_5910940767233550556[70] = 1.0;
   out_5910940767233550556[71] = 0.0;
   out_5910940767233550556[72] = 0.0;
   out_5910940767233550556[73] = 0.0;
   out_5910940767233550556[74] = 0.0;
   out_5910940767233550556[75] = 0.0;
   out_5910940767233550556[76] = 0.0;
   out_5910940767233550556[77] = 0.0;
   out_5910940767233550556[78] = 0.0;
   out_5910940767233550556[79] = 0.0;
   out_5910940767233550556[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1593669514807352017) {
   out_1593669514807352017[0] = state[0];
   out_1593669514807352017[1] = state[1];
   out_1593669514807352017[2] = state[2];
   out_1593669514807352017[3] = state[3];
   out_1593669514807352017[4] = state[4];
   out_1593669514807352017[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1593669514807352017[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1593669514807352017[7] = state[7];
   out_1593669514807352017[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4859662463778300068) {
   out_4859662463778300068[0] = 1;
   out_4859662463778300068[1] = 0;
   out_4859662463778300068[2] = 0;
   out_4859662463778300068[3] = 0;
   out_4859662463778300068[4] = 0;
   out_4859662463778300068[5] = 0;
   out_4859662463778300068[6] = 0;
   out_4859662463778300068[7] = 0;
   out_4859662463778300068[8] = 0;
   out_4859662463778300068[9] = 0;
   out_4859662463778300068[10] = 1;
   out_4859662463778300068[11] = 0;
   out_4859662463778300068[12] = 0;
   out_4859662463778300068[13] = 0;
   out_4859662463778300068[14] = 0;
   out_4859662463778300068[15] = 0;
   out_4859662463778300068[16] = 0;
   out_4859662463778300068[17] = 0;
   out_4859662463778300068[18] = 0;
   out_4859662463778300068[19] = 0;
   out_4859662463778300068[20] = 1;
   out_4859662463778300068[21] = 0;
   out_4859662463778300068[22] = 0;
   out_4859662463778300068[23] = 0;
   out_4859662463778300068[24] = 0;
   out_4859662463778300068[25] = 0;
   out_4859662463778300068[26] = 0;
   out_4859662463778300068[27] = 0;
   out_4859662463778300068[28] = 0;
   out_4859662463778300068[29] = 0;
   out_4859662463778300068[30] = 1;
   out_4859662463778300068[31] = 0;
   out_4859662463778300068[32] = 0;
   out_4859662463778300068[33] = 0;
   out_4859662463778300068[34] = 0;
   out_4859662463778300068[35] = 0;
   out_4859662463778300068[36] = 0;
   out_4859662463778300068[37] = 0;
   out_4859662463778300068[38] = 0;
   out_4859662463778300068[39] = 0;
   out_4859662463778300068[40] = 1;
   out_4859662463778300068[41] = 0;
   out_4859662463778300068[42] = 0;
   out_4859662463778300068[43] = 0;
   out_4859662463778300068[44] = 0;
   out_4859662463778300068[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4859662463778300068[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4859662463778300068[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4859662463778300068[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4859662463778300068[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4859662463778300068[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4859662463778300068[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4859662463778300068[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4859662463778300068[53] = -9.8100000000000005*dt;
   out_4859662463778300068[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4859662463778300068[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4859662463778300068[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4859662463778300068[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4859662463778300068[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4859662463778300068[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4859662463778300068[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4859662463778300068[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4859662463778300068[62] = 0;
   out_4859662463778300068[63] = 0;
   out_4859662463778300068[64] = 0;
   out_4859662463778300068[65] = 0;
   out_4859662463778300068[66] = 0;
   out_4859662463778300068[67] = 0;
   out_4859662463778300068[68] = 0;
   out_4859662463778300068[69] = 0;
   out_4859662463778300068[70] = 1;
   out_4859662463778300068[71] = 0;
   out_4859662463778300068[72] = 0;
   out_4859662463778300068[73] = 0;
   out_4859662463778300068[74] = 0;
   out_4859662463778300068[75] = 0;
   out_4859662463778300068[76] = 0;
   out_4859662463778300068[77] = 0;
   out_4859662463778300068[78] = 0;
   out_4859662463778300068[79] = 0;
   out_4859662463778300068[80] = 1;
}
void h_25(double *state, double *unused, double *out_3109249876623915000) {
   out_3109249876623915000[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2387960117244058879) {
   out_2387960117244058879[0] = 0;
   out_2387960117244058879[1] = 0;
   out_2387960117244058879[2] = 0;
   out_2387960117244058879[3] = 0;
   out_2387960117244058879[4] = 0;
   out_2387960117244058879[5] = 0;
   out_2387960117244058879[6] = 1;
   out_2387960117244058879[7] = 0;
   out_2387960117244058879[8] = 0;
}
void h_24(double *state, double *unused, double *out_8243310450403332252) {
   out_8243310450403332252[0] = state[4];
   out_8243310450403332252[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3423425513778718086) {
   out_3423425513778718086[0] = 0;
   out_3423425513778718086[1] = 0;
   out_3423425513778718086[2] = 0;
   out_3423425513778718086[3] = 0;
   out_3423425513778718086[4] = 1;
   out_3423425513778718086[5] = 0;
   out_3423425513778718086[6] = 0;
   out_3423425513778718086[7] = 0;
   out_3423425513778718086[8] = 0;
   out_3423425513778718086[9] = 0;
   out_3423425513778718086[10] = 0;
   out_3423425513778718086[11] = 0;
   out_3423425513778718086[12] = 0;
   out_3423425513778718086[13] = 0;
   out_3423425513778718086[14] = 1;
   out_3423425513778718086[15] = 0;
   out_3423425513778718086[16] = 0;
   out_3423425513778718086[17] = 0;
}
void h_30(double *state, double *unused, double *out_7444554702201113257) {
   out_7444554702201113257[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4528730224247557876) {
   out_4528730224247557876[0] = 0;
   out_4528730224247557876[1] = 0;
   out_4528730224247557876[2] = 0;
   out_4528730224247557876[3] = 0;
   out_4528730224247557876[4] = 1;
   out_4528730224247557876[5] = 0;
   out_4528730224247557876[6] = 0;
   out_4528730224247557876[7] = 0;
   out_4528730224247557876[8] = 0;
}
void h_26(double *state, double *unused, double *out_8189637268238584976) {
   out_8189637268238584976[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6129463436118115103) {
   out_6129463436118115103[0] = 0;
   out_6129463436118115103[1] = 0;
   out_6129463436118115103[2] = 0;
   out_6129463436118115103[3] = 0;
   out_6129463436118115103[4] = 0;
   out_6129463436118115103[5] = 0;
   out_6129463436118115103[6] = 0;
   out_6129463436118115103[7] = 1;
   out_6129463436118115103[8] = 0;
}
void h_27(double *state, double *unused, double *out_1758996010113418958) {
   out_1758996010113418958[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4692062376187723860) {
   out_4692062376187723860[0] = 0;
   out_4692062376187723860[1] = 0;
   out_4692062376187723860[2] = 0;
   out_4692062376187723860[3] = 1;
   out_4692062376187723860[4] = 0;
   out_4692062376187723860[5] = 0;
   out_4692062376187723860[6] = 0;
   out_4692062376187723860[7] = 0;
   out_4692062376187723860[8] = 0;
}
void h_29(double *state, double *unused, double *out_1878453518140603392) {
   out_1878453518140603392[0] = state[1];
}
void H_29(double *state, double *unused, double *out_640604185577581932) {
   out_640604185577581932[0] = 0;
   out_640604185577581932[1] = 1;
   out_640604185577581932[2] = 0;
   out_640604185577581932[3] = 0;
   out_640604185577581932[4] = 0;
   out_640604185577581932[5] = 0;
   out_640604185577581932[6] = 0;
   out_640604185577581932[7] = 0;
   out_640604185577581932[8] = 0;
}
void h_28(double *state, double *unused, double *out_7089545780969910120) {
   out_7089545780969910120[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4441794831491948642) {
   out_4441794831491948642[0] = 1;
   out_4441794831491948642[1] = 0;
   out_4441794831491948642[2] = 0;
   out_4441794831491948642[3] = 0;
   out_4441794831491948642[4] = 0;
   out_4441794831491948642[5] = 0;
   out_4441794831491948642[6] = 0;
   out_4441794831491948642[7] = 0;
   out_4441794831491948642[8] = 0;
}
void h_31(double *state, double *unused, double *out_5310037885145381436) {
   out_5310037885145381436[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2357314155367098451) {
   out_2357314155367098451[0] = 0;
   out_2357314155367098451[1] = 0;
   out_2357314155367098451[2] = 0;
   out_2357314155367098451[3] = 0;
   out_2357314155367098451[4] = 0;
   out_2357314155367098451[5] = 0;
   out_2357314155367098451[6] = 0;
   out_2357314155367098451[7] = 0;
   out_2357314155367098451[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_2515398924308937197) {
  err_fun(nom_x, delta_x, out_2515398924308937197);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5944693023187542615) {
  inv_err_fun(nom_x, true_x, out_5944693023187542615);
}
void car_H_mod_fun(double *state, double *out_5910940767233550556) {
  H_mod_fun(state, out_5910940767233550556);
}
void car_f_fun(double *state, double dt, double *out_1593669514807352017) {
  f_fun(state,  dt, out_1593669514807352017);
}
void car_F_fun(double *state, double dt, double *out_4859662463778300068) {
  F_fun(state,  dt, out_4859662463778300068);
}
void car_h_25(double *state, double *unused, double *out_3109249876623915000) {
  h_25(state, unused, out_3109249876623915000);
}
void car_H_25(double *state, double *unused, double *out_2387960117244058879) {
  H_25(state, unused, out_2387960117244058879);
}
void car_h_24(double *state, double *unused, double *out_8243310450403332252) {
  h_24(state, unused, out_8243310450403332252);
}
void car_H_24(double *state, double *unused, double *out_3423425513778718086) {
  H_24(state, unused, out_3423425513778718086);
}
void car_h_30(double *state, double *unused, double *out_7444554702201113257) {
  h_30(state, unused, out_7444554702201113257);
}
void car_H_30(double *state, double *unused, double *out_4528730224247557876) {
  H_30(state, unused, out_4528730224247557876);
}
void car_h_26(double *state, double *unused, double *out_8189637268238584976) {
  h_26(state, unused, out_8189637268238584976);
}
void car_H_26(double *state, double *unused, double *out_6129463436118115103) {
  H_26(state, unused, out_6129463436118115103);
}
void car_h_27(double *state, double *unused, double *out_1758996010113418958) {
  h_27(state, unused, out_1758996010113418958);
}
void car_H_27(double *state, double *unused, double *out_4692062376187723860) {
  H_27(state, unused, out_4692062376187723860);
}
void car_h_29(double *state, double *unused, double *out_1878453518140603392) {
  h_29(state, unused, out_1878453518140603392);
}
void car_H_29(double *state, double *unused, double *out_640604185577581932) {
  H_29(state, unused, out_640604185577581932);
}
void car_h_28(double *state, double *unused, double *out_7089545780969910120) {
  h_28(state, unused, out_7089545780969910120);
}
void car_H_28(double *state, double *unused, double *out_4441794831491948642) {
  H_28(state, unused, out_4441794831491948642);
}
void car_h_31(double *state, double *unused, double *out_5310037885145381436) {
  h_31(state, unused, out_5310037885145381436);
}
void car_H_31(double *state, double *unused, double *out_2357314155367098451) {
  H_31(state, unused, out_2357314155367098451);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
