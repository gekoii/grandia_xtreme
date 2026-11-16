#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"

void VuVecCopy(f32 *, f32 *);
void VuMtxNormalizeAxes(f32 (*)[4], f32 *);

void MgcPtclGroupAttachToPart(MgcPtclGroup *o, GphPart *n, f32 *src)
{
    FMATRIX m;
    FVECTOR v;
    FVECTOR r;
    s32 a[4];

    if (n != NULL) {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(v) : "memory");
        if (src != NULL) {
            VuVecCopy(v, src);
        }
        __asm__ volatile(
            "lqc2 $vf1, 0(%2)\n\t"
            "lqc2 $vf4, 0x0(%1)\n\t"
            "lqc2 $vf5, 0x10(%1)\n\t"
            "lqc2 $vf6, 0x20(%1)\n\t"
            "lqc2 $vf7, 0x30(%1)\n\t"
            "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
            "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
            "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
            "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(r), "r"(n->mtx), "r"(v) : "memory");
        o->pos[0] = r[0];
        o->pos[1] = r[1];
        o->pos[2] = r[2];
        VuMtxNormalizeAxes(m, n->mtx);
        VuMatrixToEuler(a, (f32 *)m);
        o->rot[0] = a[0];
        o->rot[1] = a[1];
        o->rot[2] = a[2];
    }
}
