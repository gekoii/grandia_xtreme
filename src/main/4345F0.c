/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"
#include "gph.h"


void VuMtxStackPush(s32);
void VuMtxApplyTRS(s32, f32 *, void *);
void VuMtxApplyTRSEulerYXY(s32, f32 *, void *);
void VuMtxApplyTRSEulerZXY(s32, f32 *, void *);
void VuMtxMul(f32 *, f32 *, f32 *);
s8 GphSphereClipTestScaled(f32 *, void *, f32);

void GphModelCalcMatrices(GphModel *o, f32 scale)
{
    u16 flags;
    f32 *mtx;
    f32 *m;
    s8 r;

    flags = o->flags;
    if (flags & 8) {
        return;
    }
    VuMtxStackPush(0);
    mtx = VuMtxStackGetTop();
    m = o->mtx;
    __asm__ volatile(
        "vmr32.xyzw $vf5, $vf0\n\t"
        "vmr32.xyzw $vf6, $vf5\n\t"
        "vmr32.xyzw $vf7, $vf6\n\t"
        "sqc2 $vf0, 0x30(%0)\n\t"
        "sqc2 $vf5, 0x20(%0)\n\t"
        "sqc2 $vf6, 0x10(%0)\n\t"
        "sqc2 $vf7, 0x0(%0)"
        : : "r"(m) : "memory");
    if (~flags & 7) {
        /* do-while(0) chains instead of if/else: the loop depth weights the
         * refs and so the saved-register allocation */
        do {
            if (flags & 0x8000) {
                VuMtxApplyTRSEulerYXY(~flags, m, o->pos);
                break;
            }
            if (flags & 0x4000) {
                VuMtxApplyTRSEulerZXY(~flags, m, o->pos);
                break;
            }
            VuMtxApplyTRS(~flags, m, o->pos);
        } while (0);
    }
    r = 1;
    do {
        if (scale == 0.0f) {
            r = 7;
            break;
        }
        if (!(flags & 0x80)) {
            VuMtxMul(mtx, mtx, m);
            r = GphSphereClipTestScaled(mtx, o->cullSphere, scale);
        }
    } while (0);
    o->cull = r;
    if (r >= 0) {
        __asm__ volatile(
            "lqc2 $vf1, 0x0(%1)\n\t"
            "lqc2 $vf2, 0x10(%1)\n\t"
            "lqc2 $vf3, 0x20(%1)\n\t"
            "lqc2 $vf4, 0x30(%1)\n\t"
            "sqc2 $vf1, 0x0(%0)\n\t"
            "sqc2 $vf2, 0x10(%0)\n\t"
            "sqc2 $vf3, 0x20(%0)\n\t"
            "sqc2 $vf4, 0x30(%0)"
            : : "r"(mtx), "r"(m) : "memory");
        GphModelCalcParts(o);
    }
    VuMtxStackPop();
}
