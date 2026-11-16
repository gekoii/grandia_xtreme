/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"






s32 sceMpegAddStrCallback(sceMpeg *obj, u32 reg, u64 data, void *a3, void *a4) {
    SceMpegSys *s;
    SceMpegStrCallback *e;
    u64 key;
    s32 ret;
    s32 i;

    ret = 0;
    s = obj->sys;
    e = s->strCallbacks;
    key = _sceMpegStrTypeToId(reg, data);
    for (i = 0; i < s->strCallbackCount; i++) {
        if (key == e[i].key) {
            ret = (s32)e[i].func;
            break;
        }
    }
    if (i < 64) {
        s->strCallbackCount++;
        e[i].key = key;
        e[i].data = a4;
        e[i].func = a3;
        e[i].mask = _sceMpegStrTypeTable[reg].mask;
    }
    return ret;
}
