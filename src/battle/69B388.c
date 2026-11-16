#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n29.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Q;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void FxDelayedCastShadowDraw(FxAtk *e) {
    Q q;
    FxDelayedCast *o = e->work;

    if (e->dead == 0 && o->model != 0) {
        copyq(&q, &envShadowColor);
        GphShadowSetColor(&q);
        GphDrawListSet(e->drawList);
        GphShadowDrawLayer(0);
        GphShadowDrawLayer(7);
    }
}
