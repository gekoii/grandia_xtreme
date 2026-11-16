#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void FxTraceRecordPoints(FxTraceUnit *o, s32 arg1) {
    s32 j;
    s32 k;
    FxTraceStrip *e;
    FVECTOR *sa;
    FVECTOR *sb;

    j = 0;
    for (k = 0; k < 6; k++) {
        if (o->strips[j] >= 0) {
            e = &fxTraceStrips[o->strips[j]];
            if (++e->cur >= e->count) {
                e->cur = 0;
            }
            sa = &o->ofs[j].a;
            {
                FVECTOR *d = &e->buf[e->cur].a;
                copyq(d, sa);
            }
            BtlChrPartToWorld(&e->buf[e->cur].a, arg1, o->parts[j].part0, 1);
            {
                FVECTOR *d;
                sb = &o->ofs[j].b;
                d = &e->buf[e->cur].b;
                copyq(d, sb);
            }
            BtlChrPartToWorld(&e->buf[e->cur].b, arg1, o->parts[j].part1, 1);
        }
        j++;
    }
}
