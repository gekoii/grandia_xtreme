/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

#define VADD(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VADDW(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSUB(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VMUL(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vmul.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 MapCollPushCircle(f32 *center, f32 *vel, f32 *pos, f32 radius, f32 r)
{
    FVECTOR np;
    FVECTOR d1;
    FVECTOR d0;
    FVECTOR s;
    FVECTOR perp;
    f32 len;
    f32 rad;
    f32 pen;
    f32 vx;
    f32 vz;
    f32 inv;
    f32 t;
    f32 k;

    VADD(np, pos, vel);
    VSUB(d0, np, center);
    VSUB(d1, pos, center);
    len = fsqrt(d0[0] * d0[0] + d0[2] * d0[2]);
    if (len < 0.001f) {
        vel[0] = 0.0f;
        vel[2] = 0.0f;
        return 1;
    }
    rad = r + radius;
    pen = rad - len;
    if (pen < 0.0f) {
        return 0;
    }
    vx = vel[0];
    if (vx * d1[0] + (vz = vel[2]) * d1[2] > 0.0f) {
        return 0;
    }
    if (d0[0] * vx + d0[2] * vz < 0.0f) {
        k = pen / len;
        s[2] = k;
        s[1] = k;
        s[0] = k;
        VMUL(d0, d0, s);
        d0[1] = 0.0f;
        VADDW(vel, vel, d0);
        return 1;
    }
    perp[0] = -vz;
    perp[1] = 0.0f;
    perp[2] = vx;
    vz = perp[0] * perp[0] + vx * vx;
    if (vz < 0.001f) {
        vel[0] = 0.0f;
        vel[2] = 0.0f;
        return 1;
    }
    inv = 1.0f / fsqrt(vz);
    t = (d0[0] * perp[0] + d0[2] * vx) * inv;
    if (t > 0.0f) {
        inv = inv * (rad - t);
    } else {
        inv = -inv * (rad + t);
    }
    s[2] = inv;
    s[0] = inv;
    s[1] = 0.0f;
    VMUL(s, perp, s);
    VADD(vel, vel, s);
    return 1;
}
