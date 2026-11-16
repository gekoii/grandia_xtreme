#include "common.h"
#include "protos/battle.h"
#include "sys.h"

#include "battle/misc.h"
#include "camera.h"

void VuMtxStackPush(u32);
void VuMtxStackLoad(void *);
void VuMtxMul(void *, void *, void *);
void VuMtxInverse(void *, void *);

#define COPY_MATRIX(dst, src) \
    __asm__ volatile( \
        "lqc2 $vf1, 0x0(%1)\n\t" \
        "lqc2 $vf2, 0x10(%1)\n\t" \
        "lqc2 $vf3, 0x20(%1)\n\t" \
        "lqc2 $vf4, 0x30(%1)\n\t" \
        "sqc2 $vf1, 0x0(%0)\n\t" \
        "sqc2 $vf2, 0x10(%0)\n\t" \
        "sqc2 $vf3, 0x20(%0)\n\t" \
        "sqc2 $vf4, 0x30(%0)" \
        : : "r"(dst), "r"(src) : "memory")

#define UNIT_MATRIX(m) \
    __asm__ volatile( \
        "vmr32.xyzw $vf5, $vf0\n\t" \
        "vmr32.xyzw $vf6, $vf5\n\t" \
        "vmr32.xyzw $vf7, $vf6\n\t" \
        "sqc2 $vf0, 0x30(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf6, 0x10(%0)\n\t" \
        "sqc2 $vf7, 0x0(%0)" \
        : : "r"(m) : "memory")

#define TRANS_MATRIX(dst, src, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0x30(%1)\n\t" \
        "lqc2 $vf2, 0x0(%2)\n\t" \
        "lqc2 $vf3, 0x0(%1)\n\t" \
        "lqc2 $vf4, 0x10(%1)\n\t" \
        "lqc2 $vf5, 0x20(%1)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf3, 0x0(%0)\n\t" \
        "sqc2 $vf4, 0x10(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf1, 0x30(%0)" \
        : : "r"(dst), "r"(src), "r"(v) : "memory")

void FxTraceSparkDraw(void) {
    FMATRIX save;
    FVECTOR pos;
    FMATRIX m1;
    FMATRIX m2;
    FMATRIX m3;
    FMATRIX m4;
    FxTraceSpark *e = fxTraceSparks;
    s32 i;
    s32 n;
    s8 k;

    for (i = 0; i < 16; i++, e++) {
        if (e->handle == NULL) {
            continue;
        }
        PtclBtlSprDraw(e->handle, e->count, e->unk3, 1.0f);
        if (e->unk5 == 0) {
            continue;
        }
        n = FxCloneGetCircle(pos);
        if (n <= 0) {
            e->unk5 = 0;
            continue;
        }
        COPY_MATRIX(save, sprBillboardMatrix);
        VuMtxStackPush(0);
        UNIT_MATRIX(m1);
        TRANS_MATRIX(m1, m1, pos);
        COPY_MATRIX(m2, m1);
        VuMtxInverse(m2, m2);
        for (k = 1; k <= n; k++) {
            VuMtxRotY(m3, 0x10000 / (n + 1) * k, m2);
            VuMtxMul(m3, m1, m3);
            VuMtxInverse(m4, m3);
            VuMtxMul(sprBillboardMatrix, m4, save);
            VuMtxMul(m3, btlCamera, m3);
            VuMtxStackLoad(m3);
            PtclBtlSprDraw(e->handle, e->count, e->unk3, 1.0f);
        }
        VuMtxStackPop();
        COPY_MATRIX(sprBillboardMatrix, save);
    }
}
