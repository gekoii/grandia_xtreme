#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 unkA;
    u8 padB;
    f32 *unkC;
    u8 unk10[0xA0 - 0x10];
} Obj434948;

void VuMtxStackPushCopy(void);
void VuMtxApplyTRS(s32, f32 *, void *);

void GphModelCalcPartsRest(Obj434948 *p, s32 n) {
    u16 m;
    f32 *mtx;

    do {
        m = p->unk8;
        VuMtxStackPushCopy();
        mtx = VuMtxStackGetTop();
        if (~m & 7) {
            VuMtxApplyTRS(~m, mtx, p->unk10);
        }
        __asm__ volatile(
            "lqc2 $vf1, 0x0(%1)\n\t"
            "lqc2 $vf2, 0x10(%1)\n\t"
            "lqc2 $vf3, 0x20(%1)\n\t"
            "lqc2 $vf4, 0x30(%1)\n\t"
            "sqc2 $vf1, 0x0(%0)\n\t"
            "sqc2 $vf2, 0x10(%0)\n\t"
            "sqc2 $vf3, 0x20(%0)\n\t"
            "sqc2 $vf4, 0x30(%0)"
            : : "r"(p->unkC), "r"(mtx) : "memory");
        n--;
        if (p->unkA) {
            VuMtxStackPopN(p->unkA);
        }
        p++;
    } while (n > 0);
}
