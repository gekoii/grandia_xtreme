#include "common.h"
#include "protos/battle.h"
#include "vu.h"

void VuMtxSetRotX(FMATRIX, s32);

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlVecAddRandomOffset(f32 *pos, f32 base, f32 range) {
    FVECTOR v;
    FMATRIX m;
    f32 t;

    VuMtxSetRotX(m, rand() >> 15);
    VuMtxRotY(m, rand() >> 15, m);
    v[0] = v[2] = 0.0f;
    v[3] = 1.0f;
    t = 1.0f - RND01();
    t = 1.0f - t * t;
    v[1] = base + range * t;
    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf4, 0x0(%1)\n\t"
        "lqc2 $vf5, 0x10(%1)\n\t"
        "lqc2 $vf6, 0x20(%1)\n\t"
        "lqc2 $vf7, 0x30(%1)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(m) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(pos), "r"(v) : "memory");
}
