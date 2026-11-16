#include "common.h"
#include "protos/battle.h"

#include "battle/misc.h"
#include "camera.h"

void FxTraceStripDraw(FxTraceStrip *);
void VuMtxStackPush(u32);
void VuMtxStackLoad(void *);
void VuMtxMul(void *, void *, void *);
void VuMtxInverse(void *, void *);

void FxTraceDraw(void)
{
    FVECTOR v;
    FMATRIX m;
    FMATRIX m2;
    FMATRIX m3;
    FxTraceStrip *p;
    s32 i;
    s32 n;
    s8 k;

    p = fxTraceStrips;
    for (i = 0; i < 16; i++, p++) {
        if (p->buf == NULL) {
            continue;
        }
        FxTraceStripDraw(p);
        if (p->unk18 == 0) {
            continue;
        }
        n = FxCloneGetCircle(v);
        if (n <= 0) {
            p->unk18 = 0;
            continue;
        }
        VuMtxStackPush(0);
        __asm__ volatile(
            "vmr32.xyzw $vf5, $vf0\n\t"
            "vmr32.xyzw $vf6, $vf5\n\t"
            "vmr32.xyzw $vf7, $vf6\n\t"
            "sqc2 $vf0, 0x30(%0)\n\t"
            "sqc2 $vf5, 0x20(%0)\n\t"
            "sqc2 $vf6, 0x10(%0)\n\t"
            "sqc2 $vf7, 0x0(%0)"
            : : "r"(m) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0x30(%1)\n\t"
            "lqc2 $vf2, 0x0(%2)\n\t"
            "lqc2 $vf3, 0x0(%1)\n\t"
            "lqc2 $vf4, 0x10(%1)\n\t"
            "lqc2 $vf5, 0x20(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf3, 0x0(%0)\n\t"
            "sqc2 $vf4, 0x10(%0)\n\t"
            "sqc2 $vf5, 0x20(%0)\n\t"
            "sqc2 $vf1, 0x30(%0)"
            : : "r"(m), "r"(m), "r"(v) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0x0(%1)\n\t"
            "lqc2 $vf2, 0x10(%1)\n\t"
            "lqc2 $vf3, 0x20(%1)\n\t"
            "lqc2 $vf4, 0x30(%1)\n\t"
            "sqc2 $vf1, 0x0(%0)\n\t"
            "sqc2 $vf2, 0x10(%0)\n\t"
            "sqc2 $vf3, 0x20(%0)\n\t"
            "sqc2 $vf4, 0x30(%0)"
            : : "r"(m2), "r"(m) : "memory");
        VuMtxInverse(m2, m2);
        for (k = 1; k <= n; k++) {
            VuMtxRotY(m3, 0x10000 / (n + 1) * k, m2);
            VuMtxMul(m3, m, m3);
            VuMtxMul(m3, btlCamera, m3);
            VuMtxStackLoad(m3);
            FxTraceStripDraw(p);
        }
        VuMtxStackPop();
    }
    FxTrailDraw();
    FxTraceSparkDraw();
}
