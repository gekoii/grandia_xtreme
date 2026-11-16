/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


s32 MiniBallMoveStep(MiniBall *arg0)
{
    FVECTOR tmp;
    MiniBallHit w;

    if (arg0->travel < 0.00001f) {
        return 0;
    }
    w.dist = arg0->travel;
    w.kind = 2;
    MiniBallSweepWalls(arg0, &w);
    MiniBallSweepBalls(arg0, &w);
    switch ((u32)w.kind) {
    case 0:
        MiniBallBounceWall(arg0, &w);
        break;
    case 1:
        MiniBallBounceBall(arg0, &w);
        break;
    case 2:
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "qmtc2.ni %2, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(tmp), "r"(arg0->vel), "r"(arg0->travel) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(arg0->pos), "r"(arg0->pos), "r"(tmp) : "memory");
        return 0;
    }
    return -1;
}
