#include "common.h"
#include "protos/main.h"
#include "main/n10.h"
s32 SndGetEqLevels(s32 arg0, s32 arg1) {
    SndHandle *o;
    if (sndPcmMonitorOff != 0) {
        return -10;
    }
    o = SndFindHandle(arg0);
    if (o->type != 4) {
        return -4;
    }
    SndCalcEqBands(sndPcmBuf, arg1);
    return 0;
}
