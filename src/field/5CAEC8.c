#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

typedef struct {
    s32 unk0;
    u8 pad4[8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    u8 pad40[0x40];
} Coll;


void VuVecCopy(void *, void *);
f32 VuVecDot(void *, void *);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void CamFollowResetBehind(u8 *arg0, f32 *pos)
{
    FVECTOR d;
    FVECTOR t;
    Coll c;
    f32 ang;
    f32 len;
    f32 ten;
    Coll *p;
    f32 *q;

    ang = (f32)camCtl.target->model->rot[1] * 9.58738019e-05f + 3.14159203f;
    camFollow->save.yaw = ang;
    CamCalcEyePos(pos, camFollow->unk90, ang, camFollow->pitch, camFollow->dist[0]);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyzw $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(camFollow->unk90), "r"(pos) : "memory");
    len = fsqrt(VuVecDot(d, d));
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(t), "r"(d), "r"(0.5f) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(t), "r"(pos), "r"(t) : "memory");
    FldCollGather(t, len + 100.0f);
    p = &c;
    ten = 10.0f;
    q = d;
    copyq(p->unk20, q);
    copyq(p->unk30, q);
    copyq(p->unk10, pos);
    p->unkC = ten;
    p->unk0 = 0;
    FldCollSweep(&p->unk0);
    VuVecCopy(d, c.unk30);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(camFollow->unk90), "r"(d), "r"(pos) : "memory");
    VuVecCopy(arg0 + 0x60, camFollow->unk90);
    VuVecCopy(arg0 + 0x50, pos);
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(camFollow->unkA0) : "memory");
    VuVecCopy(camFollow->unk50, pos);
}
