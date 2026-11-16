#include "common.h"
#include "protos/battle.h"

#include "battle/misc.h"

extern s8 dbgBtlTraceAttr;

void VuVecCopy(void *, void *);

s32 FxTrailCreate(s32 a0, void *a1, f32 *a2, s16 n, s8 flag) {
    FxTrail *e = fxTrails;
    s32 i;
    s32 h;
    s32 j;
    FxTraceOfs *p;

    for (i = 0; i < 32; i++, e++) {
        if (e->handle == 0) {
            break;
        }
    }
    if (i >= 32) {
        return -1;
    }
    h = BtlAlloc32(n << 5);
    e->handle = (FxTraceOfs *)h;
    if (h == 0) {
        return -1;
    }
    e->owner = (BtlUnit *)a0;
    e->unk8 = a1;
    e->unkC = 1.0f;
    if (a2 == NULL) {
        e->unk14 = 1.0f;
        e->unk10 = &e->unk14;
    } else {
        e->unk10 = a2;
    }
    e->unk20 = flag;
    e->unk28 = 5.0f;
    e->c0.w = fxTraceColors[0].start.w;
    e->c1.w = fxTraceColors[0].end.w;
    e->c0.c.r >>= 1;
    e->c1.c.r >>= 1;
    e->unk21 = -1;
    e->unk24 = 1.0f;
    e->unk18 = 0;
    e->unk1C = n;
    e->unk1A = n;
    e->unk2C = 0;
    e->unk1E = 0;
    p = e->handle;
    for (j = n; j > 0; j--) {
        VuVecCopy(p, a1);
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(p->b) : "memory");
        p->b[1] = 1.0f;
        p++;
    }
    if (dbgBtlTraceAttr >= 0) {
        FxTrailSetColorKind(i, dbgBtlTraceAttr, 1);
    }
    return i;
}
