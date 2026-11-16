/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/event.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 EvtAreaHitCapsule(f32 *m, f32 r, EvtArea *e) {
    f32 p[3];
    f32 *a = e->shapeData.capsule.a;
    f32 *b = e->shapeData.capsule.b;
    f32 dx, dy, dz;
    f32 px, py, pz;
    f32 len, inv, t;
    f32 qx, qy, qz;

    p[0] = m[0] - e->pos[0];
    p[1] = m[4] - e->pos[1];
    p[2] = m[8] - e->pos[2];
    if (e->flags & EVT_AREA_SCALED) {
        t = e->scale[0];
        a[0] *= t;
        a[1] *= t;
        a[2] *= t;
        b[0] *= t;
        b[1] *= t;
        b[2] *= t;
    }
    dx = e->shapeData.capsule.b[0] - e->shapeData.capsule.a[0];
    dy = e->shapeData.capsule.b[1] - e->shapeData.capsule.a[1];
    dz = e->shapeData.capsule.b[2] - e->shapeData.capsule.a[2];
    px = p[0] - e->shapeData.capsule.a[0];
    py = p[1] - e->shapeData.capsule.a[1];
    pz = p[2] - e->shapeData.capsule.a[2];
    len = fsqrt(dx * dx + dy * dy + dz * dz);
    inv = 1.0f / len;
    t = (dx * px + dy * py + dz * pz) * inv;
    if (len < t) {
        len = px * px + py * py + pz * pz;
        if (len < r * r) {
            return 1;
        }
        return 0;
    }
    if (t < 0.0f) {
        len = px * px + py * py + pz * pz;
        if (len < r * r) {
            return 1;
        }
        return 0;
    }
    t *= inv;
    qx = px - dx * t;
    qy = py - dy * t;
    qz = pz - dz * t;
    len = qx * qx + qy * qy + qz * qz;
    if (r * r < len) {
        return 0;
    }
    return 1;
}
