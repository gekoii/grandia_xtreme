#include "common.h"
#include "protos/battle.h"
#include "vu.h"

void VuMtxSetRotX(FMATRIX, s32);

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlVecRandomDirInRange(f32 *out, u16 xmin, u16 xrange, u16 ymin, u16 yrange) {
    FMATRIX m;

    VuMtxSetRotX(m, (u32)(xmin + RND01() * xrange));
    VuMtxRotY(m, (u32)(ymin + RND01() * yrange), m);
    do { /* do-while(0): the out[] stores keep their place (register choice for the u16 parameters) */
        out[0] = out[2] = 0.0f;
        out[1] = out[3] = 1.0f;
    } while (0);
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
        : : "r"(out), "r"(m) : "memory");
}
