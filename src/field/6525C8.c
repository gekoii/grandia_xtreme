#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void MiniBallMove(MiniBall *arg0) {
    MiniBall t;
    f32 f;

    copyq(&t.pos, &arg0->pos);
    copyq(&t.vel, &arg0->vel);
    f = arg0->speed;
    t.index = arg0->index;
    t.speed = f;
    t.travel = f;
    t.lastWall = -1;
    t.lastBall = -1;
    for (t.step = 0; MiniBallMoveStep(&t) != 0; t.step++) {
    }
    copyq(&arg0->pos, &t.pos);
    copyq(&arg0->vel, &t.vel);
    arg0->speed = t.speed;
    arg0->travel = t.speed;
}
