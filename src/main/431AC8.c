#include "common.h"
#include "camera.h"

static inline void CopyVector(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamSetShakeAmp(u8 *arg0, void *arg1, void *arg2, void *arg3) {
    Camera *c = (Camera *)arg0;

    CopyVector(c->shakeAmp, arg1);
    CopyVector(c->shakeRand, arg2);
    CopyVector(c->shakeCycle, arg3);
}
