/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

f32 VuVecNormalizeLen(f32 *, f32 *);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void MiniBallPickWinner(void) {
    MiniBallWork *f = miniBallWork;
    MiniBallGame *s = &f->game;
    FVECTOR pos;
    FVECTOR d;
    MiniBall *c;
    f32 best;
    f32 len;
    u32 i;
    u32 idx;

    if (s->reelsStopped == 1) return;
    if (miniBallData->set->pickTime != f->frame) return;
    copyq(pos, f->vec0);
    pos[0] = miniBallData->params->winX;
    pos[2] = miniBallData->params->winZ;
    best = -1.0f;
    idx = 0;
    for (i = 0; i < 20; i++) {
        c = &s->balls[i];
        if (c->state == 4) {
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf2, 0(%2)\n\t"
                "vsub.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(d), "r"(pos), "r"(c->pos) : "memory");
            len = VuVecNormalizeLen(d, d);
            if (best < 0.0f || len < best) {
                best = len;
                idx = i;
            }
        }
    }
    c = &s->balls[idx];
    c->state = 5;
    s->winColor = c->color;
    copyq(c->pos, pos);
    copyq(c->vel, miniBallWork->vec0);
    c->sprite->layer = 1;
}
