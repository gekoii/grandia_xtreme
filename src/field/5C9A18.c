/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

f32 VuVecDot(FVECTOR, FVECTOR);


void CamResetToPlayer(void) {
    FVECTOR v;
    f32 k;
    f32 t;

    camCtl.target = ChrGetByNo(1);
    switch (camCtl.aiType) {
    case 0:
        CamFollowUpdate(camField, 1);
        camField[0].pitch = 0.13962634f;
        CamApply(camField);
        t = -sinf(camField[0].yaw);
        v[0] = t * cosf(camField[0].pitch);
        v[1] = -sinf(camField[0].pitch);
        v[2] = cosf(camField[0].yaw) * cosf(camField[0].pitch);
        k = -800.0f;
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "qmtc2.ni %1, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(k) : "memory");
        CamFollowPlaceEye(camField[0].eye, camField[0].lookAt, v);
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(camField[0].eye), "r"(camField[0].lookAt) : "memory");
        camField[0].dist = sqrtf(VuVecDot(v, v));
        break;
    case 5:
        CamFollowUpdate(camField, 5);
        break;
    }
    camCtl.unk8 = 4;
    CamApply(camField);
}
