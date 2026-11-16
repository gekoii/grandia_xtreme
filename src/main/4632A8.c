#include "common.h"
#include "protos/main.h"

extern s32 sndEqFilter0[];
extern s32 sndEqFilter1[];
extern s32 sndEqFilter2[];
extern s32 sndEqFilter3[];
extern s32 sndEqFilter4[];
extern s32 sndEqFilter5[];
extern s32 sndEqFilter6[];
extern s32 sndEqFilter7[];


void SndCalcEqBands(void *arg0, s32 *arg1) {
    arg1[0] = SndCalcBandLevel(arg0, sndEqFilter0);
    arg1[1] = SndCalcBandLevel(arg0, sndEqFilter1);
    arg1[2] = SndCalcBandLevel(arg0, sndEqFilter2);
    arg1[3] = SndCalcBandLevel(arg0, sndEqFilter3);
    arg1[4] = SndCalcBandLevel(arg0, sndEqFilter4);
    arg1[5] = SndCalcBandLevel(arg0, sndEqFilter5);
    arg1[6] = SndCalcBandLevel(arg0, sndEqFilter6);
    arg1[7] = SndCalcBandLevel(arg0, sndEqFilter7);
}
