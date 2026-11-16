#include "common.h"
#include "main/n05.h"

void GphProjTexCalcMtx(GphProjTexParam *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphProjTexSetup(GphProjTexParam *arg0, FVECTOR arg1, FVECTOR arg2, FVECTOR arg3) {
    f32 s;

    copyq(arg0->dir, arg1);
    arg0->dir[3] = 1.0f;
    s = arg3[2] * 0.5f;
    arg0->pos[0] = arg2[0] + arg1[0] * s;
    arg0->pos[1] = arg2[1] + arg1[1] * s;
    arg0->pos[2] = arg2[2] + arg1[2] * s;
    arg0->pos[3] = 1.0f;
    copyq(arg0->radius, arg3);
    arg0->radius[3] = 1.0f;
    GphProjTexCalcMtx(arg0);
}
