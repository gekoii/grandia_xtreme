#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/misc.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Q;


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void FxCloneShadowDraw(FxClone *g) {
    s32 i;
    FxCloneEnt *s;
    Q q;

    if (g->unit != 0) {
        s = g->clones;
        for (i = 0; i < g->count;) {
            copyq(&q, &envShadowColor);
            q.f[3] *= s->alpha;
            s++;
            GphShadowSetColor(&q);
            GphDrawListSet(g->drawLists[i++]);
            GphShadowDrawLayer(0);
            GphShadowDrawLayer(7);
        }
    }
}
