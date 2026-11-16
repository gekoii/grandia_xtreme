/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n28.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Q;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void BtlDropShadowDraw(void) {
    Q q;
    BtlDropObj *p;
    s32 i;

    copyq(&q, &envLight.shadowColor);
    p = btlDropObjs;
    for (i = 11; i >= 0; i--, p++) {
        if (p->kind > 0 && !(p->alpha <= 0.0f)) {
            q.f[3] = envLight.shadowColor[3] * p->alpha;
            GphShadowSetColor(&q);
            GphDrawListSet(p->drawList);
            GphShadowDrawLayer(0);
            GphShadowDrawLayer(7);
        }
    }
}
