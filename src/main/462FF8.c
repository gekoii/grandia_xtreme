#include "common.h"
#include "main/n10.h"
extern s32 sndPanTable[];
void SndCalcVolPan(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5) {
    s32 t;
    s32 i = arg3 >> 2;

    t = arg0 * arg1 / 10000 * sndVolumeCurve[arg2] / 10000;
    *arg4 = t * sndPanTable[i * 2] / 10000;
    *arg5 = t * sndPanTable[i * 2 + 1] / 10000;
}
