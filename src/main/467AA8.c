#include "common.h"

void SndSoftFetchSamples(s16 *arg0, s32 *arg1) {
    {
        s32 a = arg0[0];
        s32 b = arg0[1];
        arg1[0] = a;
        arg1[8] = b;
    }
    {
        s32 a = arg0[2];
        s32 b = arg0[3];
        arg1[16] = a;
        arg1[24] = b;
    }
    {
        s32 a = arg0[4];
        s32 b = arg0[5];
        arg1[32] = a;
        arg1[40] = b;
    }
    {
        s32 a = arg0[6];
        s32 b = arg0[7];
        arg1[48] = a;
        arg1[56] = b;
    }
    {
        s32 a = arg0[8];
        s32 b = arg0[9];
        arg1[64] = a;
        arg1[72] = b;
    }
    {
        s32 a = arg0[10];
        s32 b = arg0[11];
        arg1[80] = a;
        arg1[88] = b;
    }
    {
        s32 a = arg0[12];
        s32 b = arg0[13];
        arg1[96] = a;
        arg1[104] = b;
    }
    {
        s32 a = arg0[14];
        s32 b = arg0[15];
        arg1[112] = a;
        arg1[120] = b;
    }
    {
        s32 a = arg0[16];
        s32 b = arg0[17];
        arg1[128] = a;
        arg1[136] = b;
    }
    {
        s32 a = arg0[18];
        s32 b = arg0[19];
        arg1[144] = a;
        arg1[152] = b;
    }
    {
        s32 a = arg0[20];
        s32 b = arg0[21];
        arg1[160] = a;
        arg1[168] = b;
    }
    {
        s32 a = arg0[22];
        s32 b = arg0[23];
        arg1[176] = a;
        arg1[184] = b;
    }
    {
        s32 a = arg0[24];
        s32 b = arg0[25];
        arg1[192] = a;
        arg1[200] = b;
    }
    {
        s32 a = arg0[26];
        s32 b = arg0[27];
        arg1[208] = a;
        arg1[216] = b;
    }
    {
        s32 a = arg0[28];
        s32 b = arg0[29];
        arg1[224] = a;
        arg1[232] = b;
    }
    {
        s32 b = arg0[31];
        s32 a = arg0[30];
        arg1[240] = a;
        arg1[248] = b;
    }
}
