#include "common.h"
#include "protos/main.h"
#include "gph.h"

#define UNIT_MATRIX(m) __asm__ volatile( \
    "vmr32.xyzw $vf5, $vf0\n\t" \
    "vmr32.xyzw $vf6, $vf5\n\t" \
    "vmr32.xyzw $vf7, $vf6\n\t" \
    "sqc2 $vf0, 0x30(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf6, 0x10(%0)\n\t" \
    "sqc2 $vf7, 0x0(%0)" \
    : : "r"(m) : "memory")

#define ADD_XYZ(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define TRANS_MATRIX(d, s, v) __asm__ volatile( \
    "lqc2 $vf1, 0x30(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "lqc2 $vf3, 0x0(%1)\n\t" \
    "lqc2 $vf4, 0x10(%1)\n\t" \
    "lqc2 $vf5, 0x20(%1)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf3, 0x0(%0)\n\t" \
    "sqc2 $vf4, 0x10(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf1, 0x30(%0)" \
    : : "r"(d), "r"(s), "r"(v) : "memory")

void VuMtxMul(f32 *, f32 *, f32 *);

void GphShatterMovePieces(GphModel *arg0)
{
    FMATRIX r;
    FMATRIX m;
    s32 i;
    GphPart *p;
    f32 f;

    UNIT_MATRIX(m);
    p = gphShatter.model->parts;
    for (i = 0; i < arg0->partCount; i++) {
        ADD_XYZ(gphShatterPieces[i].pos, gphShatterPieces[i].pos, gphShatterPieces[i].vel);
        f = gphShatterPieces[i].unk40;
        gphShatterPieces[i].vel[1] += f;
        gphShatterPieces[i].unk40 = f * 0.95f;
        gphShatterPieces[i].rot[0] += gphShatterPieces[i].rotVel[0];
        gphShatterPieces[i].rot[1] += gphShatterPieces[i].rotVel[1];
        gphShatterPieces[i].rot[2] += gphShatterPieces[i].rotVel[2];
        TRANS_MATRIX(p->mtx, m, gphShatterPieces[i].pos);
        VuMtxSetRotZYX((s32)r, gphShatterPieces[i].rot);
        VuMtxMul(p->mtx, p->mtx, (f32 *)r);
        p++;
    }
}
