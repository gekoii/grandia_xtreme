/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR pos;
    f32 radius;
    f32 height;
} Shape;

typedef struct {
    u8 pad0[0x7];
    u8 flags;
    u8 pad8[0x20 - 0x8];
    FVECTOR pos;
    u8 pad30[0x50 - 0x30];
    f32 scaleR;
    f32 scaleH;
    u8 pad58[0xC0 - 0x58];
    Shape shape;
} Obj;


s32 MapHtaHitCylinder(f32 *a, f32 *b, Obj *o, f32 arg3)
{
    FVECTOR v;
    Shape *s;
    f32 h;
    f32 r;
    f32 y;
    f32 lo;
    f32 hi;

    s = &o->shape;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(s->pos), "r"(o->pos) : "memory");
    r = s->radius;
    h = s->height;
    if (o->flags & 8) {
        r *= o->scaleR;
        h *= o->scaleH;
    }
    y = a[1];
    hi = y + b[1];
    lo = v[1] + h;
    if (lo <= hi) {
        return 0;
    }
    if (y < v[1] - h) {
        return 0;
    }
    if (hi < lo && lo <= y) {
        b[1] = lo - y;
        return 3;
    }
    MapCollPushCircle(v, b, a, r, arg3);
    return 2;
}
