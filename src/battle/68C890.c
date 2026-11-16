#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void FxTraceStop(s32 i, s8 j) {
    FxTraceUnit *o = &fxTraceUnits[i];
    s32 *c;
    s32 *ch;
    s32 k;
    s32 off;
    if (o->motion != 0) {
        off = j * 4;
        ch = o->strips;
        c = (s32 *)((u8 *)ch + off);
        if (*c >= 0) {
            o->flags[j] = 1;
            FxTraceStripFree(*c);
            *c = -1;
        }
        for (k = 0; k < 6; k++) {
            if (ch[k] >= 0) {
                return;
            }
        }
        o->motion = 0;
    }
}
