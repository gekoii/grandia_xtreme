#include "common.h"
#include "protos/main.h"
#include "camera.h"



#define VCLEAR(v) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(v) : "memory")

void CamUpdateShake(Camera *p)
{
    VCLEAR(p->shakeOfs);
    if (p->shakeFlags & 1) {
        if (p->shakeMode & 0x10) {
            if (p->shakeTime == 0) {
                p->shakeTick = 0;
                p->shakeFlags &= ~1;
                VCLEAR(p->shakeOfs);
                return;
            }
            p->shakeTime--;
        }
        CamCalcShake(p);
    }
}
