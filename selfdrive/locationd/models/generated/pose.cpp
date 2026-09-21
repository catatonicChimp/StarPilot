#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6754834019914573692) {
   out_6754834019914573692[0] = delta_x[0] + nom_x[0];
   out_6754834019914573692[1] = delta_x[1] + nom_x[1];
   out_6754834019914573692[2] = delta_x[2] + nom_x[2];
   out_6754834019914573692[3] = delta_x[3] + nom_x[3];
   out_6754834019914573692[4] = delta_x[4] + nom_x[4];
   out_6754834019914573692[5] = delta_x[5] + nom_x[5];
   out_6754834019914573692[6] = delta_x[6] + nom_x[6];
   out_6754834019914573692[7] = delta_x[7] + nom_x[7];
   out_6754834019914573692[8] = delta_x[8] + nom_x[8];
   out_6754834019914573692[9] = delta_x[9] + nom_x[9];
   out_6754834019914573692[10] = delta_x[10] + nom_x[10];
   out_6754834019914573692[11] = delta_x[11] + nom_x[11];
   out_6754834019914573692[12] = delta_x[12] + nom_x[12];
   out_6754834019914573692[13] = delta_x[13] + nom_x[13];
   out_6754834019914573692[14] = delta_x[14] + nom_x[14];
   out_6754834019914573692[15] = delta_x[15] + nom_x[15];
   out_6754834019914573692[16] = delta_x[16] + nom_x[16];
   out_6754834019914573692[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1942039733683991315) {
   out_1942039733683991315[0] = -nom_x[0] + true_x[0];
   out_1942039733683991315[1] = -nom_x[1] + true_x[1];
   out_1942039733683991315[2] = -nom_x[2] + true_x[2];
   out_1942039733683991315[3] = -nom_x[3] + true_x[3];
   out_1942039733683991315[4] = -nom_x[4] + true_x[4];
   out_1942039733683991315[5] = -nom_x[5] + true_x[5];
   out_1942039733683991315[6] = -nom_x[6] + true_x[6];
   out_1942039733683991315[7] = -nom_x[7] + true_x[7];
   out_1942039733683991315[8] = -nom_x[8] + true_x[8];
   out_1942039733683991315[9] = -nom_x[9] + true_x[9];
   out_1942039733683991315[10] = -nom_x[10] + true_x[10];
   out_1942039733683991315[11] = -nom_x[11] + true_x[11];
   out_1942039733683991315[12] = -nom_x[12] + true_x[12];
   out_1942039733683991315[13] = -nom_x[13] + true_x[13];
   out_1942039733683991315[14] = -nom_x[14] + true_x[14];
   out_1942039733683991315[15] = -nom_x[15] + true_x[15];
   out_1942039733683991315[16] = -nom_x[16] + true_x[16];
   out_1942039733683991315[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7857849580727627842) {
   out_7857849580727627842[0] = 1.0;
   out_7857849580727627842[1] = 0.0;
   out_7857849580727627842[2] = 0.0;
   out_7857849580727627842[3] = 0.0;
   out_7857849580727627842[4] = 0.0;
   out_7857849580727627842[5] = 0.0;
   out_7857849580727627842[6] = 0.0;
   out_7857849580727627842[7] = 0.0;
   out_7857849580727627842[8] = 0.0;
   out_7857849580727627842[9] = 0.0;
   out_7857849580727627842[10] = 0.0;
   out_7857849580727627842[11] = 0.0;
   out_7857849580727627842[12] = 0.0;
   out_7857849580727627842[13] = 0.0;
   out_7857849580727627842[14] = 0.0;
   out_7857849580727627842[15] = 0.0;
   out_7857849580727627842[16] = 0.0;
   out_7857849580727627842[17] = 0.0;
   out_7857849580727627842[18] = 0.0;
   out_7857849580727627842[19] = 1.0;
   out_7857849580727627842[20] = 0.0;
   out_7857849580727627842[21] = 0.0;
   out_7857849580727627842[22] = 0.0;
   out_7857849580727627842[23] = 0.0;
   out_7857849580727627842[24] = 0.0;
   out_7857849580727627842[25] = 0.0;
   out_7857849580727627842[26] = 0.0;
   out_7857849580727627842[27] = 0.0;
   out_7857849580727627842[28] = 0.0;
   out_7857849580727627842[29] = 0.0;
   out_7857849580727627842[30] = 0.0;
   out_7857849580727627842[31] = 0.0;
   out_7857849580727627842[32] = 0.0;
   out_7857849580727627842[33] = 0.0;
   out_7857849580727627842[34] = 0.0;
   out_7857849580727627842[35] = 0.0;
   out_7857849580727627842[36] = 0.0;
   out_7857849580727627842[37] = 0.0;
   out_7857849580727627842[38] = 1.0;
   out_7857849580727627842[39] = 0.0;
   out_7857849580727627842[40] = 0.0;
   out_7857849580727627842[41] = 0.0;
   out_7857849580727627842[42] = 0.0;
   out_7857849580727627842[43] = 0.0;
   out_7857849580727627842[44] = 0.0;
   out_7857849580727627842[45] = 0.0;
   out_7857849580727627842[46] = 0.0;
   out_7857849580727627842[47] = 0.0;
   out_7857849580727627842[48] = 0.0;
   out_7857849580727627842[49] = 0.0;
   out_7857849580727627842[50] = 0.0;
   out_7857849580727627842[51] = 0.0;
   out_7857849580727627842[52] = 0.0;
   out_7857849580727627842[53] = 0.0;
   out_7857849580727627842[54] = 0.0;
   out_7857849580727627842[55] = 0.0;
   out_7857849580727627842[56] = 0.0;
   out_7857849580727627842[57] = 1.0;
   out_7857849580727627842[58] = 0.0;
   out_7857849580727627842[59] = 0.0;
   out_7857849580727627842[60] = 0.0;
   out_7857849580727627842[61] = 0.0;
   out_7857849580727627842[62] = 0.0;
   out_7857849580727627842[63] = 0.0;
   out_7857849580727627842[64] = 0.0;
   out_7857849580727627842[65] = 0.0;
   out_7857849580727627842[66] = 0.0;
   out_7857849580727627842[67] = 0.0;
   out_7857849580727627842[68] = 0.0;
   out_7857849580727627842[69] = 0.0;
   out_7857849580727627842[70] = 0.0;
   out_7857849580727627842[71] = 0.0;
   out_7857849580727627842[72] = 0.0;
   out_7857849580727627842[73] = 0.0;
   out_7857849580727627842[74] = 0.0;
   out_7857849580727627842[75] = 0.0;
   out_7857849580727627842[76] = 1.0;
   out_7857849580727627842[77] = 0.0;
   out_7857849580727627842[78] = 0.0;
   out_7857849580727627842[79] = 0.0;
   out_7857849580727627842[80] = 0.0;
   out_7857849580727627842[81] = 0.0;
   out_7857849580727627842[82] = 0.0;
   out_7857849580727627842[83] = 0.0;
   out_7857849580727627842[84] = 0.0;
   out_7857849580727627842[85] = 0.0;
   out_7857849580727627842[86] = 0.0;
   out_7857849580727627842[87] = 0.0;
   out_7857849580727627842[88] = 0.0;
   out_7857849580727627842[89] = 0.0;
   out_7857849580727627842[90] = 0.0;
   out_7857849580727627842[91] = 0.0;
   out_7857849580727627842[92] = 0.0;
   out_7857849580727627842[93] = 0.0;
   out_7857849580727627842[94] = 0.0;
   out_7857849580727627842[95] = 1.0;
   out_7857849580727627842[96] = 0.0;
   out_7857849580727627842[97] = 0.0;
   out_7857849580727627842[98] = 0.0;
   out_7857849580727627842[99] = 0.0;
   out_7857849580727627842[100] = 0.0;
   out_7857849580727627842[101] = 0.0;
   out_7857849580727627842[102] = 0.0;
   out_7857849580727627842[103] = 0.0;
   out_7857849580727627842[104] = 0.0;
   out_7857849580727627842[105] = 0.0;
   out_7857849580727627842[106] = 0.0;
   out_7857849580727627842[107] = 0.0;
   out_7857849580727627842[108] = 0.0;
   out_7857849580727627842[109] = 0.0;
   out_7857849580727627842[110] = 0.0;
   out_7857849580727627842[111] = 0.0;
   out_7857849580727627842[112] = 0.0;
   out_7857849580727627842[113] = 0.0;
   out_7857849580727627842[114] = 1.0;
   out_7857849580727627842[115] = 0.0;
   out_7857849580727627842[116] = 0.0;
   out_7857849580727627842[117] = 0.0;
   out_7857849580727627842[118] = 0.0;
   out_7857849580727627842[119] = 0.0;
   out_7857849580727627842[120] = 0.0;
   out_7857849580727627842[121] = 0.0;
   out_7857849580727627842[122] = 0.0;
   out_7857849580727627842[123] = 0.0;
   out_7857849580727627842[124] = 0.0;
   out_7857849580727627842[125] = 0.0;
   out_7857849580727627842[126] = 0.0;
   out_7857849580727627842[127] = 0.0;
   out_7857849580727627842[128] = 0.0;
   out_7857849580727627842[129] = 0.0;
   out_7857849580727627842[130] = 0.0;
   out_7857849580727627842[131] = 0.0;
   out_7857849580727627842[132] = 0.0;
   out_7857849580727627842[133] = 1.0;
   out_7857849580727627842[134] = 0.0;
   out_7857849580727627842[135] = 0.0;
   out_7857849580727627842[136] = 0.0;
   out_7857849580727627842[137] = 0.0;
   out_7857849580727627842[138] = 0.0;
   out_7857849580727627842[139] = 0.0;
   out_7857849580727627842[140] = 0.0;
   out_7857849580727627842[141] = 0.0;
   out_7857849580727627842[142] = 0.0;
   out_7857849580727627842[143] = 0.0;
   out_7857849580727627842[144] = 0.0;
   out_7857849580727627842[145] = 0.0;
   out_7857849580727627842[146] = 0.0;
   out_7857849580727627842[147] = 0.0;
   out_7857849580727627842[148] = 0.0;
   out_7857849580727627842[149] = 0.0;
   out_7857849580727627842[150] = 0.0;
   out_7857849580727627842[151] = 0.0;
   out_7857849580727627842[152] = 1.0;
   out_7857849580727627842[153] = 0.0;
   out_7857849580727627842[154] = 0.0;
   out_7857849580727627842[155] = 0.0;
   out_7857849580727627842[156] = 0.0;
   out_7857849580727627842[157] = 0.0;
   out_7857849580727627842[158] = 0.0;
   out_7857849580727627842[159] = 0.0;
   out_7857849580727627842[160] = 0.0;
   out_7857849580727627842[161] = 0.0;
   out_7857849580727627842[162] = 0.0;
   out_7857849580727627842[163] = 0.0;
   out_7857849580727627842[164] = 0.0;
   out_7857849580727627842[165] = 0.0;
   out_7857849580727627842[166] = 0.0;
   out_7857849580727627842[167] = 0.0;
   out_7857849580727627842[168] = 0.0;
   out_7857849580727627842[169] = 0.0;
   out_7857849580727627842[170] = 0.0;
   out_7857849580727627842[171] = 1.0;
   out_7857849580727627842[172] = 0.0;
   out_7857849580727627842[173] = 0.0;
   out_7857849580727627842[174] = 0.0;
   out_7857849580727627842[175] = 0.0;
   out_7857849580727627842[176] = 0.0;
   out_7857849580727627842[177] = 0.0;
   out_7857849580727627842[178] = 0.0;
   out_7857849580727627842[179] = 0.0;
   out_7857849580727627842[180] = 0.0;
   out_7857849580727627842[181] = 0.0;
   out_7857849580727627842[182] = 0.0;
   out_7857849580727627842[183] = 0.0;
   out_7857849580727627842[184] = 0.0;
   out_7857849580727627842[185] = 0.0;
   out_7857849580727627842[186] = 0.0;
   out_7857849580727627842[187] = 0.0;
   out_7857849580727627842[188] = 0.0;
   out_7857849580727627842[189] = 0.0;
   out_7857849580727627842[190] = 1.0;
   out_7857849580727627842[191] = 0.0;
   out_7857849580727627842[192] = 0.0;
   out_7857849580727627842[193] = 0.0;
   out_7857849580727627842[194] = 0.0;
   out_7857849580727627842[195] = 0.0;
   out_7857849580727627842[196] = 0.0;
   out_7857849580727627842[197] = 0.0;
   out_7857849580727627842[198] = 0.0;
   out_7857849580727627842[199] = 0.0;
   out_7857849580727627842[200] = 0.0;
   out_7857849580727627842[201] = 0.0;
   out_7857849580727627842[202] = 0.0;
   out_7857849580727627842[203] = 0.0;
   out_7857849580727627842[204] = 0.0;
   out_7857849580727627842[205] = 0.0;
   out_7857849580727627842[206] = 0.0;
   out_7857849580727627842[207] = 0.0;
   out_7857849580727627842[208] = 0.0;
   out_7857849580727627842[209] = 1.0;
   out_7857849580727627842[210] = 0.0;
   out_7857849580727627842[211] = 0.0;
   out_7857849580727627842[212] = 0.0;
   out_7857849580727627842[213] = 0.0;
   out_7857849580727627842[214] = 0.0;
   out_7857849580727627842[215] = 0.0;
   out_7857849580727627842[216] = 0.0;
   out_7857849580727627842[217] = 0.0;
   out_7857849580727627842[218] = 0.0;
   out_7857849580727627842[219] = 0.0;
   out_7857849580727627842[220] = 0.0;
   out_7857849580727627842[221] = 0.0;
   out_7857849580727627842[222] = 0.0;
   out_7857849580727627842[223] = 0.0;
   out_7857849580727627842[224] = 0.0;
   out_7857849580727627842[225] = 0.0;
   out_7857849580727627842[226] = 0.0;
   out_7857849580727627842[227] = 0.0;
   out_7857849580727627842[228] = 1.0;
   out_7857849580727627842[229] = 0.0;
   out_7857849580727627842[230] = 0.0;
   out_7857849580727627842[231] = 0.0;
   out_7857849580727627842[232] = 0.0;
   out_7857849580727627842[233] = 0.0;
   out_7857849580727627842[234] = 0.0;
   out_7857849580727627842[235] = 0.0;
   out_7857849580727627842[236] = 0.0;
   out_7857849580727627842[237] = 0.0;
   out_7857849580727627842[238] = 0.0;
   out_7857849580727627842[239] = 0.0;
   out_7857849580727627842[240] = 0.0;
   out_7857849580727627842[241] = 0.0;
   out_7857849580727627842[242] = 0.0;
   out_7857849580727627842[243] = 0.0;
   out_7857849580727627842[244] = 0.0;
   out_7857849580727627842[245] = 0.0;
   out_7857849580727627842[246] = 0.0;
   out_7857849580727627842[247] = 1.0;
   out_7857849580727627842[248] = 0.0;
   out_7857849580727627842[249] = 0.0;
   out_7857849580727627842[250] = 0.0;
   out_7857849580727627842[251] = 0.0;
   out_7857849580727627842[252] = 0.0;
   out_7857849580727627842[253] = 0.0;
   out_7857849580727627842[254] = 0.0;
   out_7857849580727627842[255] = 0.0;
   out_7857849580727627842[256] = 0.0;
   out_7857849580727627842[257] = 0.0;
   out_7857849580727627842[258] = 0.0;
   out_7857849580727627842[259] = 0.0;
   out_7857849580727627842[260] = 0.0;
   out_7857849580727627842[261] = 0.0;
   out_7857849580727627842[262] = 0.0;
   out_7857849580727627842[263] = 0.0;
   out_7857849580727627842[264] = 0.0;
   out_7857849580727627842[265] = 0.0;
   out_7857849580727627842[266] = 1.0;
   out_7857849580727627842[267] = 0.0;
   out_7857849580727627842[268] = 0.0;
   out_7857849580727627842[269] = 0.0;
   out_7857849580727627842[270] = 0.0;
   out_7857849580727627842[271] = 0.0;
   out_7857849580727627842[272] = 0.0;
   out_7857849580727627842[273] = 0.0;
   out_7857849580727627842[274] = 0.0;
   out_7857849580727627842[275] = 0.0;
   out_7857849580727627842[276] = 0.0;
   out_7857849580727627842[277] = 0.0;
   out_7857849580727627842[278] = 0.0;
   out_7857849580727627842[279] = 0.0;
   out_7857849580727627842[280] = 0.0;
   out_7857849580727627842[281] = 0.0;
   out_7857849580727627842[282] = 0.0;
   out_7857849580727627842[283] = 0.0;
   out_7857849580727627842[284] = 0.0;
   out_7857849580727627842[285] = 1.0;
   out_7857849580727627842[286] = 0.0;
   out_7857849580727627842[287] = 0.0;
   out_7857849580727627842[288] = 0.0;
   out_7857849580727627842[289] = 0.0;
   out_7857849580727627842[290] = 0.0;
   out_7857849580727627842[291] = 0.0;
   out_7857849580727627842[292] = 0.0;
   out_7857849580727627842[293] = 0.0;
   out_7857849580727627842[294] = 0.0;
   out_7857849580727627842[295] = 0.0;
   out_7857849580727627842[296] = 0.0;
   out_7857849580727627842[297] = 0.0;
   out_7857849580727627842[298] = 0.0;
   out_7857849580727627842[299] = 0.0;
   out_7857849580727627842[300] = 0.0;
   out_7857849580727627842[301] = 0.0;
   out_7857849580727627842[302] = 0.0;
   out_7857849580727627842[303] = 0.0;
   out_7857849580727627842[304] = 1.0;
   out_7857849580727627842[305] = 0.0;
   out_7857849580727627842[306] = 0.0;
   out_7857849580727627842[307] = 0.0;
   out_7857849580727627842[308] = 0.0;
   out_7857849580727627842[309] = 0.0;
   out_7857849580727627842[310] = 0.0;
   out_7857849580727627842[311] = 0.0;
   out_7857849580727627842[312] = 0.0;
   out_7857849580727627842[313] = 0.0;
   out_7857849580727627842[314] = 0.0;
   out_7857849580727627842[315] = 0.0;
   out_7857849580727627842[316] = 0.0;
   out_7857849580727627842[317] = 0.0;
   out_7857849580727627842[318] = 0.0;
   out_7857849580727627842[319] = 0.0;
   out_7857849580727627842[320] = 0.0;
   out_7857849580727627842[321] = 0.0;
   out_7857849580727627842[322] = 0.0;
   out_7857849580727627842[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5326446350083772854) {
   out_5326446350083772854[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5326446350083772854[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5326446350083772854[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5326446350083772854[3] = dt*state[12] + state[3];
   out_5326446350083772854[4] = dt*state[13] + state[4];
   out_5326446350083772854[5] = dt*state[14] + state[5];
   out_5326446350083772854[6] = state[6];
   out_5326446350083772854[7] = state[7];
   out_5326446350083772854[8] = state[8];
   out_5326446350083772854[9] = state[9];
   out_5326446350083772854[10] = state[10];
   out_5326446350083772854[11] = state[11];
   out_5326446350083772854[12] = state[12];
   out_5326446350083772854[13] = state[13];
   out_5326446350083772854[14] = state[14];
   out_5326446350083772854[15] = state[15];
   out_5326446350083772854[16] = state[16];
   out_5326446350083772854[17] = state[17];
}
void F_fun(double *state, double dt, double *out_5404773275233196712) {
   out_5404773275233196712[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5404773275233196712[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5404773275233196712[2] = 0;
   out_5404773275233196712[3] = 0;
   out_5404773275233196712[4] = 0;
   out_5404773275233196712[5] = 0;
   out_5404773275233196712[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5404773275233196712[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5404773275233196712[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5404773275233196712[9] = 0;
   out_5404773275233196712[10] = 0;
   out_5404773275233196712[11] = 0;
   out_5404773275233196712[12] = 0;
   out_5404773275233196712[13] = 0;
   out_5404773275233196712[14] = 0;
   out_5404773275233196712[15] = 0;
   out_5404773275233196712[16] = 0;
   out_5404773275233196712[17] = 0;
   out_5404773275233196712[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5404773275233196712[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5404773275233196712[20] = 0;
   out_5404773275233196712[21] = 0;
   out_5404773275233196712[22] = 0;
   out_5404773275233196712[23] = 0;
   out_5404773275233196712[24] = 0;
   out_5404773275233196712[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5404773275233196712[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5404773275233196712[27] = 0;
   out_5404773275233196712[28] = 0;
   out_5404773275233196712[29] = 0;
   out_5404773275233196712[30] = 0;
   out_5404773275233196712[31] = 0;
   out_5404773275233196712[32] = 0;
   out_5404773275233196712[33] = 0;
   out_5404773275233196712[34] = 0;
   out_5404773275233196712[35] = 0;
   out_5404773275233196712[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5404773275233196712[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5404773275233196712[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5404773275233196712[39] = 0;
   out_5404773275233196712[40] = 0;
   out_5404773275233196712[41] = 0;
   out_5404773275233196712[42] = 0;
   out_5404773275233196712[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5404773275233196712[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5404773275233196712[45] = 0;
   out_5404773275233196712[46] = 0;
   out_5404773275233196712[47] = 0;
   out_5404773275233196712[48] = 0;
   out_5404773275233196712[49] = 0;
   out_5404773275233196712[50] = 0;
   out_5404773275233196712[51] = 0;
   out_5404773275233196712[52] = 0;
   out_5404773275233196712[53] = 0;
   out_5404773275233196712[54] = 0;
   out_5404773275233196712[55] = 0;
   out_5404773275233196712[56] = 0;
   out_5404773275233196712[57] = 1;
   out_5404773275233196712[58] = 0;
   out_5404773275233196712[59] = 0;
   out_5404773275233196712[60] = 0;
   out_5404773275233196712[61] = 0;
   out_5404773275233196712[62] = 0;
   out_5404773275233196712[63] = 0;
   out_5404773275233196712[64] = 0;
   out_5404773275233196712[65] = 0;
   out_5404773275233196712[66] = dt;
   out_5404773275233196712[67] = 0;
   out_5404773275233196712[68] = 0;
   out_5404773275233196712[69] = 0;
   out_5404773275233196712[70] = 0;
   out_5404773275233196712[71] = 0;
   out_5404773275233196712[72] = 0;
   out_5404773275233196712[73] = 0;
   out_5404773275233196712[74] = 0;
   out_5404773275233196712[75] = 0;
   out_5404773275233196712[76] = 1;
   out_5404773275233196712[77] = 0;
   out_5404773275233196712[78] = 0;
   out_5404773275233196712[79] = 0;
   out_5404773275233196712[80] = 0;
   out_5404773275233196712[81] = 0;
   out_5404773275233196712[82] = 0;
   out_5404773275233196712[83] = 0;
   out_5404773275233196712[84] = 0;
   out_5404773275233196712[85] = dt;
   out_5404773275233196712[86] = 0;
   out_5404773275233196712[87] = 0;
   out_5404773275233196712[88] = 0;
   out_5404773275233196712[89] = 0;
   out_5404773275233196712[90] = 0;
   out_5404773275233196712[91] = 0;
   out_5404773275233196712[92] = 0;
   out_5404773275233196712[93] = 0;
   out_5404773275233196712[94] = 0;
   out_5404773275233196712[95] = 1;
   out_5404773275233196712[96] = 0;
   out_5404773275233196712[97] = 0;
   out_5404773275233196712[98] = 0;
   out_5404773275233196712[99] = 0;
   out_5404773275233196712[100] = 0;
   out_5404773275233196712[101] = 0;
   out_5404773275233196712[102] = 0;
   out_5404773275233196712[103] = 0;
   out_5404773275233196712[104] = dt;
   out_5404773275233196712[105] = 0;
   out_5404773275233196712[106] = 0;
   out_5404773275233196712[107] = 0;
   out_5404773275233196712[108] = 0;
   out_5404773275233196712[109] = 0;
   out_5404773275233196712[110] = 0;
   out_5404773275233196712[111] = 0;
   out_5404773275233196712[112] = 0;
   out_5404773275233196712[113] = 0;
   out_5404773275233196712[114] = 1;
   out_5404773275233196712[115] = 0;
   out_5404773275233196712[116] = 0;
   out_5404773275233196712[117] = 0;
   out_5404773275233196712[118] = 0;
   out_5404773275233196712[119] = 0;
   out_5404773275233196712[120] = 0;
   out_5404773275233196712[121] = 0;
   out_5404773275233196712[122] = 0;
   out_5404773275233196712[123] = 0;
   out_5404773275233196712[124] = 0;
   out_5404773275233196712[125] = 0;
   out_5404773275233196712[126] = 0;
   out_5404773275233196712[127] = 0;
   out_5404773275233196712[128] = 0;
   out_5404773275233196712[129] = 0;
   out_5404773275233196712[130] = 0;
   out_5404773275233196712[131] = 0;
   out_5404773275233196712[132] = 0;
   out_5404773275233196712[133] = 1;
   out_5404773275233196712[134] = 0;
   out_5404773275233196712[135] = 0;
   out_5404773275233196712[136] = 0;
   out_5404773275233196712[137] = 0;
   out_5404773275233196712[138] = 0;
   out_5404773275233196712[139] = 0;
   out_5404773275233196712[140] = 0;
   out_5404773275233196712[141] = 0;
   out_5404773275233196712[142] = 0;
   out_5404773275233196712[143] = 0;
   out_5404773275233196712[144] = 0;
   out_5404773275233196712[145] = 0;
   out_5404773275233196712[146] = 0;
   out_5404773275233196712[147] = 0;
   out_5404773275233196712[148] = 0;
   out_5404773275233196712[149] = 0;
   out_5404773275233196712[150] = 0;
   out_5404773275233196712[151] = 0;
   out_5404773275233196712[152] = 1;
   out_5404773275233196712[153] = 0;
   out_5404773275233196712[154] = 0;
   out_5404773275233196712[155] = 0;
   out_5404773275233196712[156] = 0;
   out_5404773275233196712[157] = 0;
   out_5404773275233196712[158] = 0;
   out_5404773275233196712[159] = 0;
   out_5404773275233196712[160] = 0;
   out_5404773275233196712[161] = 0;
   out_5404773275233196712[162] = 0;
   out_5404773275233196712[163] = 0;
   out_5404773275233196712[164] = 0;
   out_5404773275233196712[165] = 0;
   out_5404773275233196712[166] = 0;
   out_5404773275233196712[167] = 0;
   out_5404773275233196712[168] = 0;
   out_5404773275233196712[169] = 0;
   out_5404773275233196712[170] = 0;
   out_5404773275233196712[171] = 1;
   out_5404773275233196712[172] = 0;
   out_5404773275233196712[173] = 0;
   out_5404773275233196712[174] = 0;
   out_5404773275233196712[175] = 0;
   out_5404773275233196712[176] = 0;
   out_5404773275233196712[177] = 0;
   out_5404773275233196712[178] = 0;
   out_5404773275233196712[179] = 0;
   out_5404773275233196712[180] = 0;
   out_5404773275233196712[181] = 0;
   out_5404773275233196712[182] = 0;
   out_5404773275233196712[183] = 0;
   out_5404773275233196712[184] = 0;
   out_5404773275233196712[185] = 0;
   out_5404773275233196712[186] = 0;
   out_5404773275233196712[187] = 0;
   out_5404773275233196712[188] = 0;
   out_5404773275233196712[189] = 0;
   out_5404773275233196712[190] = 1;
   out_5404773275233196712[191] = 0;
   out_5404773275233196712[192] = 0;
   out_5404773275233196712[193] = 0;
   out_5404773275233196712[194] = 0;
   out_5404773275233196712[195] = 0;
   out_5404773275233196712[196] = 0;
   out_5404773275233196712[197] = 0;
   out_5404773275233196712[198] = 0;
   out_5404773275233196712[199] = 0;
   out_5404773275233196712[200] = 0;
   out_5404773275233196712[201] = 0;
   out_5404773275233196712[202] = 0;
   out_5404773275233196712[203] = 0;
   out_5404773275233196712[204] = 0;
   out_5404773275233196712[205] = 0;
   out_5404773275233196712[206] = 0;
   out_5404773275233196712[207] = 0;
   out_5404773275233196712[208] = 0;
   out_5404773275233196712[209] = 1;
   out_5404773275233196712[210] = 0;
   out_5404773275233196712[211] = 0;
   out_5404773275233196712[212] = 0;
   out_5404773275233196712[213] = 0;
   out_5404773275233196712[214] = 0;
   out_5404773275233196712[215] = 0;
   out_5404773275233196712[216] = 0;
   out_5404773275233196712[217] = 0;
   out_5404773275233196712[218] = 0;
   out_5404773275233196712[219] = 0;
   out_5404773275233196712[220] = 0;
   out_5404773275233196712[221] = 0;
   out_5404773275233196712[222] = 0;
   out_5404773275233196712[223] = 0;
   out_5404773275233196712[224] = 0;
   out_5404773275233196712[225] = 0;
   out_5404773275233196712[226] = 0;
   out_5404773275233196712[227] = 0;
   out_5404773275233196712[228] = 1;
   out_5404773275233196712[229] = 0;
   out_5404773275233196712[230] = 0;
   out_5404773275233196712[231] = 0;
   out_5404773275233196712[232] = 0;
   out_5404773275233196712[233] = 0;
   out_5404773275233196712[234] = 0;
   out_5404773275233196712[235] = 0;
   out_5404773275233196712[236] = 0;
   out_5404773275233196712[237] = 0;
   out_5404773275233196712[238] = 0;
   out_5404773275233196712[239] = 0;
   out_5404773275233196712[240] = 0;
   out_5404773275233196712[241] = 0;
   out_5404773275233196712[242] = 0;
   out_5404773275233196712[243] = 0;
   out_5404773275233196712[244] = 0;
   out_5404773275233196712[245] = 0;
   out_5404773275233196712[246] = 0;
   out_5404773275233196712[247] = 1;
   out_5404773275233196712[248] = 0;
   out_5404773275233196712[249] = 0;
   out_5404773275233196712[250] = 0;
   out_5404773275233196712[251] = 0;
   out_5404773275233196712[252] = 0;
   out_5404773275233196712[253] = 0;
   out_5404773275233196712[254] = 0;
   out_5404773275233196712[255] = 0;
   out_5404773275233196712[256] = 0;
   out_5404773275233196712[257] = 0;
   out_5404773275233196712[258] = 0;
   out_5404773275233196712[259] = 0;
   out_5404773275233196712[260] = 0;
   out_5404773275233196712[261] = 0;
   out_5404773275233196712[262] = 0;
   out_5404773275233196712[263] = 0;
   out_5404773275233196712[264] = 0;
   out_5404773275233196712[265] = 0;
   out_5404773275233196712[266] = 1;
   out_5404773275233196712[267] = 0;
   out_5404773275233196712[268] = 0;
   out_5404773275233196712[269] = 0;
   out_5404773275233196712[270] = 0;
   out_5404773275233196712[271] = 0;
   out_5404773275233196712[272] = 0;
   out_5404773275233196712[273] = 0;
   out_5404773275233196712[274] = 0;
   out_5404773275233196712[275] = 0;
   out_5404773275233196712[276] = 0;
   out_5404773275233196712[277] = 0;
   out_5404773275233196712[278] = 0;
   out_5404773275233196712[279] = 0;
   out_5404773275233196712[280] = 0;
   out_5404773275233196712[281] = 0;
   out_5404773275233196712[282] = 0;
   out_5404773275233196712[283] = 0;
   out_5404773275233196712[284] = 0;
   out_5404773275233196712[285] = 1;
   out_5404773275233196712[286] = 0;
   out_5404773275233196712[287] = 0;
   out_5404773275233196712[288] = 0;
   out_5404773275233196712[289] = 0;
   out_5404773275233196712[290] = 0;
   out_5404773275233196712[291] = 0;
   out_5404773275233196712[292] = 0;
   out_5404773275233196712[293] = 0;
   out_5404773275233196712[294] = 0;
   out_5404773275233196712[295] = 0;
   out_5404773275233196712[296] = 0;
   out_5404773275233196712[297] = 0;
   out_5404773275233196712[298] = 0;
   out_5404773275233196712[299] = 0;
   out_5404773275233196712[300] = 0;
   out_5404773275233196712[301] = 0;
   out_5404773275233196712[302] = 0;
   out_5404773275233196712[303] = 0;
   out_5404773275233196712[304] = 1;
   out_5404773275233196712[305] = 0;
   out_5404773275233196712[306] = 0;
   out_5404773275233196712[307] = 0;
   out_5404773275233196712[308] = 0;
   out_5404773275233196712[309] = 0;
   out_5404773275233196712[310] = 0;
   out_5404773275233196712[311] = 0;
   out_5404773275233196712[312] = 0;
   out_5404773275233196712[313] = 0;
   out_5404773275233196712[314] = 0;
   out_5404773275233196712[315] = 0;
   out_5404773275233196712[316] = 0;
   out_5404773275233196712[317] = 0;
   out_5404773275233196712[318] = 0;
   out_5404773275233196712[319] = 0;
   out_5404773275233196712[320] = 0;
   out_5404773275233196712[321] = 0;
   out_5404773275233196712[322] = 0;
   out_5404773275233196712[323] = 1;
}
void h_4(double *state, double *unused, double *out_5757806951307091776) {
   out_5757806951307091776[0] = state[6] + state[9];
   out_5757806951307091776[1] = state[7] + state[10];
   out_5757806951307091776[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1086512085330193500) {
   out_1086512085330193500[0] = 0;
   out_1086512085330193500[1] = 0;
   out_1086512085330193500[2] = 0;
   out_1086512085330193500[3] = 0;
   out_1086512085330193500[4] = 0;
   out_1086512085330193500[5] = 0;
   out_1086512085330193500[6] = 1;
   out_1086512085330193500[7] = 0;
   out_1086512085330193500[8] = 0;
   out_1086512085330193500[9] = 1;
   out_1086512085330193500[10] = 0;
   out_1086512085330193500[11] = 0;
   out_1086512085330193500[12] = 0;
   out_1086512085330193500[13] = 0;
   out_1086512085330193500[14] = 0;
   out_1086512085330193500[15] = 0;
   out_1086512085330193500[16] = 0;
   out_1086512085330193500[17] = 0;
   out_1086512085330193500[18] = 0;
   out_1086512085330193500[19] = 0;
   out_1086512085330193500[20] = 0;
   out_1086512085330193500[21] = 0;
   out_1086512085330193500[22] = 0;
   out_1086512085330193500[23] = 0;
   out_1086512085330193500[24] = 0;
   out_1086512085330193500[25] = 1;
   out_1086512085330193500[26] = 0;
   out_1086512085330193500[27] = 0;
   out_1086512085330193500[28] = 1;
   out_1086512085330193500[29] = 0;
   out_1086512085330193500[30] = 0;
   out_1086512085330193500[31] = 0;
   out_1086512085330193500[32] = 0;
   out_1086512085330193500[33] = 0;
   out_1086512085330193500[34] = 0;
   out_1086512085330193500[35] = 0;
   out_1086512085330193500[36] = 0;
   out_1086512085330193500[37] = 0;
   out_1086512085330193500[38] = 0;
   out_1086512085330193500[39] = 0;
   out_1086512085330193500[40] = 0;
   out_1086512085330193500[41] = 0;
   out_1086512085330193500[42] = 0;
   out_1086512085330193500[43] = 0;
   out_1086512085330193500[44] = 1;
   out_1086512085330193500[45] = 0;
   out_1086512085330193500[46] = 0;
   out_1086512085330193500[47] = 1;
   out_1086512085330193500[48] = 0;
   out_1086512085330193500[49] = 0;
   out_1086512085330193500[50] = 0;
   out_1086512085330193500[51] = 0;
   out_1086512085330193500[52] = 0;
   out_1086512085330193500[53] = 0;
}
void h_10(double *state, double *unused, double *out_6766273307206545632) {
   out_6766273307206545632[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_6766273307206545632[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_6766273307206545632[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5690926367066679972) {
   out_5690926367066679972[0] = 0;
   out_5690926367066679972[1] = 9.8100000000000005*cos(state[1]);
   out_5690926367066679972[2] = 0;
   out_5690926367066679972[3] = 0;
   out_5690926367066679972[4] = -state[8];
   out_5690926367066679972[5] = state[7];
   out_5690926367066679972[6] = 0;
   out_5690926367066679972[7] = state[5];
   out_5690926367066679972[8] = -state[4];
   out_5690926367066679972[9] = 0;
   out_5690926367066679972[10] = 0;
   out_5690926367066679972[11] = 0;
   out_5690926367066679972[12] = 1;
   out_5690926367066679972[13] = 0;
   out_5690926367066679972[14] = 0;
   out_5690926367066679972[15] = 1;
   out_5690926367066679972[16] = 0;
   out_5690926367066679972[17] = 0;
   out_5690926367066679972[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5690926367066679972[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5690926367066679972[20] = 0;
   out_5690926367066679972[21] = state[8];
   out_5690926367066679972[22] = 0;
   out_5690926367066679972[23] = -state[6];
   out_5690926367066679972[24] = -state[5];
   out_5690926367066679972[25] = 0;
   out_5690926367066679972[26] = state[3];
   out_5690926367066679972[27] = 0;
   out_5690926367066679972[28] = 0;
   out_5690926367066679972[29] = 0;
   out_5690926367066679972[30] = 0;
   out_5690926367066679972[31] = 1;
   out_5690926367066679972[32] = 0;
   out_5690926367066679972[33] = 0;
   out_5690926367066679972[34] = 1;
   out_5690926367066679972[35] = 0;
   out_5690926367066679972[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5690926367066679972[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5690926367066679972[38] = 0;
   out_5690926367066679972[39] = -state[7];
   out_5690926367066679972[40] = state[6];
   out_5690926367066679972[41] = 0;
   out_5690926367066679972[42] = state[4];
   out_5690926367066679972[43] = -state[3];
   out_5690926367066679972[44] = 0;
   out_5690926367066679972[45] = 0;
   out_5690926367066679972[46] = 0;
   out_5690926367066679972[47] = 0;
   out_5690926367066679972[48] = 0;
   out_5690926367066679972[49] = 0;
   out_5690926367066679972[50] = 1;
   out_5690926367066679972[51] = 0;
   out_5690926367066679972[52] = 0;
   out_5690926367066679972[53] = 1;
}
void h_13(double *state, double *unused, double *out_5663744141387295532) {
   out_5663744141387295532[0] = state[3];
   out_5663744141387295532[1] = state[4];
   out_5663744141387295532[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4920267548632717524) {
   out_4920267548632717524[0] = 0;
   out_4920267548632717524[1] = 0;
   out_4920267548632717524[2] = 0;
   out_4920267548632717524[3] = 1;
   out_4920267548632717524[4] = 0;
   out_4920267548632717524[5] = 0;
   out_4920267548632717524[6] = 0;
   out_4920267548632717524[7] = 0;
   out_4920267548632717524[8] = 0;
   out_4920267548632717524[9] = 0;
   out_4920267548632717524[10] = 0;
   out_4920267548632717524[11] = 0;
   out_4920267548632717524[12] = 0;
   out_4920267548632717524[13] = 0;
   out_4920267548632717524[14] = 0;
   out_4920267548632717524[15] = 0;
   out_4920267548632717524[16] = 0;
   out_4920267548632717524[17] = 0;
   out_4920267548632717524[18] = 0;
   out_4920267548632717524[19] = 0;
   out_4920267548632717524[20] = 0;
   out_4920267548632717524[21] = 0;
   out_4920267548632717524[22] = 1;
   out_4920267548632717524[23] = 0;
   out_4920267548632717524[24] = 0;
   out_4920267548632717524[25] = 0;
   out_4920267548632717524[26] = 0;
   out_4920267548632717524[27] = 0;
   out_4920267548632717524[28] = 0;
   out_4920267548632717524[29] = 0;
   out_4920267548632717524[30] = 0;
   out_4920267548632717524[31] = 0;
   out_4920267548632717524[32] = 0;
   out_4920267548632717524[33] = 0;
   out_4920267548632717524[34] = 0;
   out_4920267548632717524[35] = 0;
   out_4920267548632717524[36] = 0;
   out_4920267548632717524[37] = 0;
   out_4920267548632717524[38] = 0;
   out_4920267548632717524[39] = 0;
   out_4920267548632717524[40] = 0;
   out_4920267548632717524[41] = 1;
   out_4920267548632717524[42] = 0;
   out_4920267548632717524[43] = 0;
   out_4920267548632717524[44] = 0;
   out_4920267548632717524[45] = 0;
   out_4920267548632717524[46] = 0;
   out_4920267548632717524[47] = 0;
   out_4920267548632717524[48] = 0;
   out_4920267548632717524[49] = 0;
   out_4920267548632717524[50] = 0;
   out_4920267548632717524[51] = 0;
   out_4920267548632717524[52] = 0;
   out_4920267548632717524[53] = 0;
}
void h_14(double *state, double *unused, double *out_2121763880548370869) {
   out_2121763880548370869[0] = state[6];
   out_2121763880548370869[1] = state[7];
   out_2121763880548370869[2] = state[8];
}
void H_14(double *state, double *unused, double *out_4169300517625565796) {
   out_4169300517625565796[0] = 0;
   out_4169300517625565796[1] = 0;
   out_4169300517625565796[2] = 0;
   out_4169300517625565796[3] = 0;
   out_4169300517625565796[4] = 0;
   out_4169300517625565796[5] = 0;
   out_4169300517625565796[6] = 1;
   out_4169300517625565796[7] = 0;
   out_4169300517625565796[8] = 0;
   out_4169300517625565796[9] = 0;
   out_4169300517625565796[10] = 0;
   out_4169300517625565796[11] = 0;
   out_4169300517625565796[12] = 0;
   out_4169300517625565796[13] = 0;
   out_4169300517625565796[14] = 0;
   out_4169300517625565796[15] = 0;
   out_4169300517625565796[16] = 0;
   out_4169300517625565796[17] = 0;
   out_4169300517625565796[18] = 0;
   out_4169300517625565796[19] = 0;
   out_4169300517625565796[20] = 0;
   out_4169300517625565796[21] = 0;
   out_4169300517625565796[22] = 0;
   out_4169300517625565796[23] = 0;
   out_4169300517625565796[24] = 0;
   out_4169300517625565796[25] = 1;
   out_4169300517625565796[26] = 0;
   out_4169300517625565796[27] = 0;
   out_4169300517625565796[28] = 0;
   out_4169300517625565796[29] = 0;
   out_4169300517625565796[30] = 0;
   out_4169300517625565796[31] = 0;
   out_4169300517625565796[32] = 0;
   out_4169300517625565796[33] = 0;
   out_4169300517625565796[34] = 0;
   out_4169300517625565796[35] = 0;
   out_4169300517625565796[36] = 0;
   out_4169300517625565796[37] = 0;
   out_4169300517625565796[38] = 0;
   out_4169300517625565796[39] = 0;
   out_4169300517625565796[40] = 0;
   out_4169300517625565796[41] = 0;
   out_4169300517625565796[42] = 0;
   out_4169300517625565796[43] = 0;
   out_4169300517625565796[44] = 1;
   out_4169300517625565796[45] = 0;
   out_4169300517625565796[46] = 0;
   out_4169300517625565796[47] = 0;
   out_4169300517625565796[48] = 0;
   out_4169300517625565796[49] = 0;
   out_4169300517625565796[50] = 0;
   out_4169300517625565796[51] = 0;
   out_4169300517625565796[52] = 0;
   out_4169300517625565796[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_6754834019914573692) {
  err_fun(nom_x, delta_x, out_6754834019914573692);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1942039733683991315) {
  inv_err_fun(nom_x, true_x, out_1942039733683991315);
}
void pose_H_mod_fun(double *state, double *out_7857849580727627842) {
  H_mod_fun(state, out_7857849580727627842);
}
void pose_f_fun(double *state, double dt, double *out_5326446350083772854) {
  f_fun(state,  dt, out_5326446350083772854);
}
void pose_F_fun(double *state, double dt, double *out_5404773275233196712) {
  F_fun(state,  dt, out_5404773275233196712);
}
void pose_h_4(double *state, double *unused, double *out_5757806951307091776) {
  h_4(state, unused, out_5757806951307091776);
}
void pose_H_4(double *state, double *unused, double *out_1086512085330193500) {
  H_4(state, unused, out_1086512085330193500);
}
void pose_h_10(double *state, double *unused, double *out_6766273307206545632) {
  h_10(state, unused, out_6766273307206545632);
}
void pose_H_10(double *state, double *unused, double *out_5690926367066679972) {
  H_10(state, unused, out_5690926367066679972);
}
void pose_h_13(double *state, double *unused, double *out_5663744141387295532) {
  h_13(state, unused, out_5663744141387295532);
}
void pose_H_13(double *state, double *unused, double *out_4920267548632717524) {
  H_13(state, unused, out_4920267548632717524);
}
void pose_h_14(double *state, double *unused, double *out_2121763880548370869) {
  h_14(state, unused, out_2121763880548370869);
}
void pose_H_14(double *state, double *unused, double *out_4169300517625565796) {
  H_14(state, unused, out_4169300517625565796);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
