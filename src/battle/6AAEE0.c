#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"



static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void BtlChrPartToWorld(f32 *out, BtlUnit *u, GphPart *n, s8 keep)
{
    if (keep == 0) {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(out) : "memory");
    }
    if ((u32)u->id < 16 && u->state != 0) {
        if (u->motion == NULL || n == NULL) {
            if (u->body >= 0) {
                BtlChrModelUpdate(&btlUnits[u->body]);
            }
            copyq(out, u->pos);
        } else {
            BtlChrModelUpdate(u);
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf4, 0x0(%0)\n\t"
                "lqc2 $vf5, 0x10(%0)\n\t"
                "lqc2 $vf6, 0x20(%0)\n\t"
                "lqc2 $vf7, 0x30(%0)\n\t"
                "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
                "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
                "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
                "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
                "sqc2 $vf1, 0(%1)"
                : : "r"(n->mtx), "r"(out) : "memory");
        }
    }
}
