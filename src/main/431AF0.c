#include "common.h"
#include "camera.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamSetShakePeriod(u8 *arg0, s32 arg1, void *arg2, void *arg3) {
    Camera *c = (Camera *)arg0;

    c->shakePeriod = arg1;
    copyq(c->shakeAmp2, arg2);
    copyq(c->shakeRand2, arg3);
}
