#include "common.h"
#include "protos/battle.h"

#include "battle/n30.h"

void BtlVecRandomDir(f32 *);

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void FxCastSparkSpawn(f32 *base) {
    FxCastSpark *p = fxCastSparks;
    s32 i;
    f32 s;

    if (fxCastSparkCount >= 50) {
        return;
    }
    for (i = 0; i < 50; i++, p++) {
        if (p->life <= 0) {
            break;
        }
    }
    if (i < 50) {
        p->age = 0;
        fxCastSparkCount++;
        p->life = RND01() * 20.0f + 40.0f;
        BtlVecRandomDir(&p->pos.x);
        p->accel.x = p->pos.x * 0.1f;
        p->accel.y = p->pos.y * 0.1f;
        p->accel.z = p->pos.z * 0.1f;
        s = RND01() * 2.0f + 1.0f;
        p->vel.x = p->pos.x * s;
        p->vel.y = p->pos.y * s;
        p->vel.z = p->pos.z * s;
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "qmtc2.ni %1, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(&p->pos), "r"(100.0f) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "lqc2 $vf2, 0(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(&p->pos), "r"(base) : "memory");
        p->alpha = 0.0f;
        p->scale = RND01() + 0.5f;
    }
}
