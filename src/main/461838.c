#include "common.h"
#include "protos/main.h"
#include "main/n10.h"

s32 SndGetOutputLevel(s32 arg0, s32 *arg1, s32 *arg2) {
    s16 *p;
    s16 *q;
    s32 i;
    s32 l;
    s32 r;

    if (sndPcmMonitorOff != 0) {
        return -10;
    }
    if (SndFindHandle(arg0)->type != 4) {
        return -4;
    }
    r = 0;
    p = sndPcmBuf;
    l = 0;
    q = p + 512;
    for (i = 0; i < 512; i++) {
        s32 t = *p++;
        l += (t * t) >> 8;
        t = *q++;
        r += (t * t) >> 8;
    }
    p += 512;
    q += 512;
    for (i = 0; i < 512; i++) {
        s32 t = *p++;
        l += (t * t) >> 8;
        t = *q++;
        r += (t * t) >> 8;
    }
    *arg1 = log10f(SndIntToFloat(l) + 1.0) * 20.0;
    *arg2 = log10f(SndIntToFloat(r) + 1.0) * 20.0;
    return 0;
}
