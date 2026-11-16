#include "common.h"
#include "battle/misc.h"
#include "battle/n28.h"
#include "protos/battle.h"

s32 FxTraceStripAlloc(s32 n, s32 k, s8 flag) {
    FxTraceStrip *e = fxTraceStrips;
    s32 i;
    s32 h;
    if (dbgBtlTraceAttr >= 0) {
        k = dbgBtlTraceAttr;
    }
    if (k >= 11) {
        k = 0;
    }
    for (i = 0; i < 16; i++, e++) {
        if (e->buf == 0) {
            break;
        }
    }
    if (i >= 16) {
        return -1;
    }
    h = BtlAlloc32(n << 5);
    e->buf = h;
    if (h == 0) {
        return -1;
    }
    e->count = n;
    e->unk14 = flag;
    e->unk15 = 2;
    e->c0.w = fxTraceColors[k].start.w;
    e->c1.w = fxTraceColors[k].end.w;
    e->unk10 = 0;
    e->cur = 0;
    e->unk16 = k;
    e->unk18 = 0;
    if (ptclBurstTypeTable[k].unk0 != 0) {
        e->unk17 = FxTraceSparkAlloc(k);
    } else {
        e->unk17 = -1;
    }
    return i;
}
