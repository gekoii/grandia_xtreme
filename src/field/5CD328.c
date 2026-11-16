/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"


f32 VuVecDot(void *, void *);
void VuVecNormalize(void *, void *);
s32 CamFollowTrailMove(f32 *, void *, void *);
s32 CamRayTest(f32 *, void *, void *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void CamFollowBlocked(f32 *arg0, void *arg1, void *arg2)
{
    FVECTOR v;
    f32 len;

    camFollowState = 0;
    CamTrailGet(CamTrailCount(), v);
    if (camFollowCur->turn != 0.0f) {
        CamFollowBlockedTurn(arg0, arg1, arg2);
    } else {
        camFollowState |= 0x10;
        CamFollowTrailMove(arg0, arg1, arg2);
        len = fsqrt(VuVecDot(arg0, arg0));
        CamRayTest(arg0, arg1, v);
        VuVecNormalize(arg0, arg0);
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "qmtc2.ni %2, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(arg0), "r"(arg0), "r"(len) : "memory");
    }
}
