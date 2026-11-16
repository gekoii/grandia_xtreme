#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "protos/battle.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void FxTraceResume(BtlUnit *u) {
    FxTraceUnit *o = &fxTraceUnits[u->id];
    s32 j;
    s32 k;
    f32 s;
    FxTraceStrip *e;
    if (o->motion != 0) {
        return;
    }
    for (j = 0; j < 6; j++) {
        if (o->flags[j] != 0) {
            break;
        }
    }
    if (j >= 6) {
        return;
    }
    copyq(&o->pos, &u->pos);
    copyq(&o->rot, &u->rot);
    copyq(&o->scale, &u->scale);
    o->frame = u->frame;
    o->motion = u->motion;
    o->frameSpeed = u->frameSpeed;
    s = 1.0f;
    if (u->id < 4 && u->motionNo == 9) {
        s = 2.0f;
    }
    o->step = s;
    for (j = 0; j < 6; j++) {
        if (o->flags[j] != 0) {
            o->strips[j] = FxTraceStripAlloc(o->width * o->height, o->kinds[j], o->height);
            if (o->strips[j] >= 0 && FxCloneIsCircleOf(u) != 0) {
                e = &fxTraceStrips[o->strips[j]];
                k = e->unk17;
                e->unk18 = 1;
                if (k >= 0) {
                    fxTraceSparks[k].unk5 = 1;
                }
            }
        }
    }
}
