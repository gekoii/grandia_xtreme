/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"

typedef struct {
    f32 x;
    f32 z;
    f32 r;
} Circle;

typedef struct {
    f32 ax;
    f32 az;
    f32 bx;
    f32 bz;
} Pts;

void VuVecNormalize(f32 *, f32 *);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define SQ(x) ((x) * (x))

s32 BtlCircleSegIntersect(Pts *out, Circle *c, BtlSeg *s) {
    Pts p;
    f32 v[3];
    f32 *pv;
    f32 m1, b1, m2, b2;
    f32 ix, iz;
    f32 d;
    s32 res;

    pv = v;
    v[0] = s->z0 - s->z1;
    v[1] = s->x1 - s->x0;
    v[2] = 0.0f;
    VuVecNormalize(pv, pv);
    p.ax = c->x + v[0] * c->r;
    p.az = c->z + v[1] * c->r;
    p.bx = c->x - v[0] * c->r;
    p.bz = c->z - v[1] * c->r;
    m1 = (s->z1 - s->z0) / (s->x1 - s->x0);
    m2 = (p.bz - p.az) / (p.bx - p.ax);
    b1 = s->z0 - m1 * s->x0;
    b2 = p.az - m2 * p.ax;
    if (s->x1 - s->x0 == 0.0f || p.bz - p.az == 0.0f) {
        ix = s->x0;
        iz = p.az;
    } else if (p.bx - p.ax == 0.0f || s->z1 - s->z0 == 0.0f) {
        ix = p.ax;
        iz = s->z0;
    } else {
        ix = (b2 - b1) / (m1 - m2);
        iz = m1 * ix + b1;
    }
    res = 0;
    d = c->r - fsqrt(SQ(ix - c->x) + SQ(iz - c->z));
    if (d < 0.0f) {
        return 0;
    }
    v[0] = s->x1 - s->x0;
    v[1] = s->z1 - s->z0;
    v[2] = 0.0f;
    VuVecNormalize(pv, pv);
    p.ax = ix + v[0] * d;
    p.az = iz + v[1] * d;
    p.bx = ix - v[0] * d;
    p.bz = iz - v[1] * d;
    if (s->x1 - s->x0 == 0.0f) {
        if ((s->z0 <= p.az && p.az <= s->z1) || (p.az <= s->z0 && s->z1 <= p.az)) {
            res = 1;
        }
        if ((s->z0 <= p.bz && p.bz <= s->z1) || (p.bz <= s->z0 && s->z1 <= p.bz)) {
            res ^= 2;
        }
    } else {
        if ((s->x0 <= p.ax && p.ax <= s->x1) || (p.ax <= s->x0 && s->x1 <= p.ax)) {
            res = 1;
        }
        if ((s->x0 <= p.bx && p.bx <= s->x1) || (p.bx <= s->x0 && s->x1 <= p.bx)) {
            res ^= 2;
        }
    }
    if (out != NULL) {
        *out = p;
    }
    return res;
}
