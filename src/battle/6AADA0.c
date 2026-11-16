#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"



static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void BtlChrGetPartPos(f32 *out, BtlUnit *u, s32 idx)
{
    s32 n;
    GphModel *o;
    f32 *mt;

    if (idx < 0x1000) {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(out) : "memory");
    } else {
        idx &= 0xFFF;
    }
    if ((u32)u->id < 16 && u->state != 0) {
        if (u->motion == NULL) {
            if (u->body >= 0) {
                BtlChrModelUpdate(&btlUnits[u->body]);
            }
            copyq(out, u->pos);
        } else {
            BtlChrModelUpdate(u);
            if (u->parts[0] >= 0 && idx == 1) {
                ((s32 (*)(f32 *, BtlUnit *, s32, s32))BtlChrPartToWorld)(out, u, BtlChrFindPart(u->model, 0x3E9), 1);
            } else {
                o = u->model;
                mt = o->restMtx;
                if (idx >= 0) {
                    n = o->partCount - 1;
                    if (n >= idx) {
                        n = idx;
                    }
                } else {
                    n = 0;
                }
                mt += n * 16;
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
                    : : "r"(mt), "r"(out) : "memory");
            }
        }
    }
}
