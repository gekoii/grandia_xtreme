#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "party.h"
#include "protos/battle.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void FxTraceStart(BtlUnit *u, s32 j, s32 w, s32 h, s32 kind) {
    FxTraceUnit *o = &fxTraceUnits[u->id];
    PtyTraceDef *fx;
    s32 cnt;
    s32 k;
    s32 t;
    s32 *c;
    s32 *ch;
    s32 off;
    f32 s;
    FxTraceStrip *e;
    PtyCharDef *df;
    BtlChrData *d;
    if (u->id < 4) {
        /* &ptyCharDefTable[id - 0x1001] written out: the plain index differs */
        df = (PtyCharDef *)(u->params->id * 0x7C + (u32)ptyCharDefTable - 0x7C07C);
        cnt = df->traceCount;
        fx = df->traces;
    } else {
        d = u->def->data;
        cnt = d->traceCount;
        fx = d->traces;
        if (u->params->id == 0x1A1 && j == 0 && u->parts[0] >= 0) {
            j = 4;
            if (btlUnits[u->parts[0]].def->data->unk0 != 0x1A3) {
                j = 0;
            }
        }
    }
    if (j >= cnt) {
        return;
    }
    fx += j;
    FxTraceSetParts(u->id, j, fx->parts[0], fx->parts[1]);
    FxTraceSetOffset(u->id, j, 0, fx->offset0[0], fx->offset0[1], fx->offset0[2]);
    FxTraceSetOffset(u->id, j, 1, fx->offset1[0], fx->offset1[1], fx->offset1[2]);
    off = j * 4;
    if (o->motion == 0) {
        o->width = w;
        o->height = h;
        copyq(&o->pos, &u->pos);
        copyq(&o->rot, &u->rot);
        copyq(&o->scale, &u->scale);
        o->frame = u->frame;
        o->motion = u->motion;
        o->frameSpeed = u->frameSpeed;
        o->kinds[j] = kind;
        ch = o->strips;
        for (k = 0; k < 6; k++) {
            ch[k] = -1;
            o->flags[k] = 0;
            o->kinds[k] = kind;
        }
    } else {
        ch = o->strips;
    }
    o->t = 0.0f;
    s = 1.0f;
    if (u->id < 4 && u->motionNo == 9) {
        s = 2.0f;
    }
    o->step = s;
    c = &ch[j];
    if (*c < 0) {
        *c = FxTraceStripAlloc(w * h, kind, h);
        if (*c < 0) {
            return;
        }
    }
    if (FxCloneIsCircleOf(u) != 0) {
        e = &fxTraceStrips[*c];
        t = e->unk17;
        e->unk18 = 1;
        if (t >= 0) {
            fxTraceSparks[t].unk5 = 1;
        }
    }
}
