#include "common.h"
#include "pad.h"



s32 DbgMenuLineSelect(s32 arg0, s32 arg1) {
    Pad *p = &padPort1;
    s32 v;

    dbgPad = p;
    v = arg0;
    if (p->repeatFast & 0x1000) {
        v = arg0 - 1;
    }
    if (p->repeatFast & 0x4000) {
        v = v + 1;
    }
    if (v < 0) {
        v = arg1;
    }
    if (arg1 < v) {
        v = 0;
    }
    return v;
}
