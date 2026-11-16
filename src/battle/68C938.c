#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "protos/battle.h"

void FxTraceSetParts(s32 i, s32 j, s32 a, s32 b) {
    s32 h = btlUnits[i].model;
    u8 *o = (u8 *)&fxTraceUnits[i];
    s32 r;
    u8 *q;
    s32 v;
    s32 off = 0x4C;
    if (h != 0) {
        r = BtlChrFindPart(h, a);
        q = o + j * 8 + 0x48;
        *(s32 *)q = r;
        o = o + off + j * 8;
        if (a == b) {
            v = r;
        } else {
            v = BtlChrFindPart(h, b);
        }
        *(s32 *)o = v;
    }
}
