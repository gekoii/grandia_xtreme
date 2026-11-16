/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef struct {
    u8 pad0[0x28];
    f32 unk28;
    f32 unk2C;
    u8 pad30[0x4];
    f32 unk34;
    u8 pad38[0x4];
    f32 unk3C;
    u8 pad40[0x4];
    f32 unk44;
    u8 pad48[0x4];
    f32 unk4C;
} Camera;

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 GphSphereClipTest(f32 (*m)[4], f32 *v)
{
    Camera *c;
    f32 *p;
    f32 r;
    f32 z;
    f32 ax;
    f32 ay;
    s32 ret;
    f32 c28, c2C, c34, c3C, c44, c4C;

    c = *(Camera **)0x700000EC;
    p = (f32 *)0x70003000;
    c2C = c->unk2C;
    c28 = c->unk28;
    c44 = c->unk44;
    c4C = c->unk4C;
    c34 = c->unk34;
    c3C = c->unk3C;
    copyq(p, v);
    r = v[3];
    p[3] = 1.0f;
    __asm__ volatile(
        "lqc2 $vf1, 0x0(%0)\n\t"
        "lqc2 $vf4, 0x0(%1)\n\t"
        "lqc2 $vf5, 0x10(%1)\n\t"
        "lqc2 $vf6, 0x20(%1)\n\t"
        "lqc2 $vf7, 0x30(%1)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0x0(%0)"
        : : "r"(p), "r"(m) : "memory");
    z = p[2];
    ay = __builtin_fabsf(p[1]);
    ax = __builtin_fabsf(p[0]);
    if (z + r < 0.0f) {
        return -1;
    }
    if (c44 * z + r * c34 < ax) {
        return -1;
    }
    if (c4C * z + r * c3C < ay) {
        return -1;
    }
    if (c2C < z - r) {
        return -1;
    }
    if (z + r < c28) {
        return -1;
    }
    if (c44 * z < ax + r * c34) {
        return 1;
    }
    if (c4C * z < ay + r * c3C) {
        return 1;
    }
    ret = 0;
    if (c2C < z + r) {
        ret = 1;
    }
    return ret;
}
