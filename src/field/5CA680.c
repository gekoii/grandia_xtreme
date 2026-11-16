#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

typedef struct {
    s32 unk0;
    u8 pad4[8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR vel;
    FVECTOR push;
    u8 pad40[0x40];
} Mover;

f32 VuVecDot(FVECTOR, FVECTOR);
f32 VuVecNormalizeLen(FVECTOR, FVECTOR);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void CamFollowPlaceEye(FVECTOR arg0, FVECTOR arg1, FVECTOR arg2) {
    FVECTOR a;
    Mover m;
    f32 d;
    f32 k;
    f32 t;
    Mover *mp;
    f32 ten;

    if (camCtl.aiType == 0) {
        d = fsqrt(VuVecDot(arg2, arg2));
        k = 0.5f;
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "qmtc2.ni %2, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(a), "r"(arg2), "r"(k) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(a), "r"(a), "r"(arg1) : "memory");
        FldCollGather(a, d + 100.0f);
        mp = &m;
        ten = 10.0f;
        copyq(m.vel, arg2);
        copyq(m.push, arg2);
        copyq(m.unk10, arg1);
        mp->unkC = ten;
        m.unk0 = 0;
        FldCollSweep(mp);
        t = VuVecNormalizeLen(a, m.push) - 1.0f;
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "qmtc2.ni %2, $vf2\n\t"
            "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(a), "r"(a), "r"(t) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(arg0), "r"(arg1), "r"(a) : "memory");
        copyq(camFollow->unk50, arg1);
        copyq(camFollow->unk60, arg1);
        copyq(camFollow->unk70, arg1);
        copyq(camFollow->unk80, arg0);
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(camFollow->unkA0) : "memory");
        camFollow->turn = 0.0f;
    }
    camPadTurn.l = 0.0f;
    camPadTurn.r = 0.0f;
}
