/* Port-only C for MapHtaHitCapsule (0x005E5950), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"

/* The capsule's height above each end point is shapeData.box[8] (+0xE0),
   past the two end points (event.h's capsule view has only those). The
   caller, MapHtaHitArea (src/field/5E50E0.c), declares this without the
   f32: the game passes its own r through $f12 untouched. */
s32 MapHtaHitCapsule(f32 *pos, f32 *v, EvtArea *area, f32 r) {
    f32 *a = area->shapeData.capsule.a;
    f32 *b = area->shapeData.capsule.b;
    f32 h = area->shapeData.box[8];
    FVECTOR pt;
    f32 lx, ly, lz;
    f32 dx, dz;
    f32 rx, rz;
    f32 nx, nz;
    f32 ex, ez;
    f32 vx, vz;
    f32 x0, z0;
    f32 t, d, k, len;

    lx = pos[0] - area->pos[0];
    ly = pos[1] - area->pos[1];
    lz = pos[2] - area->pos[2];
    if (!(a[1] - 30.0f < ly) && !(b[1] - 30.0f < ly)) {
        return 0;
    }
    if (!(ly < a[1] + h) && !(ly < b[1] + h)) {
        return 0;
    }

    /* Unit axis (dx, dz) and the normal (nx, nz) on the mover's side */
    dx = b[0] - a[0];
    dz = b[2] - a[2];
    len = __builtin_sqrtf(dx * dx + dz * dz);
    k = 1.0f / len;
    rx = lx - a[0];
    rz = lz - a[2];
    x0 = a[0];
    z0 = a[2];
    dz *= k;
    dx *= k;
    nx = -dz;
    nz = dx;
    if (rx * nx + rz * nz < 0.0f) {
        nz = -nz;
        nx = dz;
    }
    ex = v[0] + rx;
    ez = v[2] + rz;
    if (r < ex * nx + ez * nz) {
        return 0;
    }
    t = ex * dx + ez * dz;

    /* Past an end: push off that end's circle */
    if (len < t) {
        pt[1] = 0.0f;
        pt[2] = area->pos[2] + b[2];
        pt[0] = area->pos[0] + b[0];
        return MapCollPushCircle(pt, v, pos, 0.0f, r);
    }
    if (t < 0.0f) {
        pt[1] = 0.0f;
        pt[2] = area->pos[2] + a[2];
        pt[0] = area->pos[0] + a[0];
        return MapCollPushCircle(pt, v, pos, 0.0f, r);
    }

    /* Along the side: slide */
    rx = lx - x0;
    rz = lz - z0;
    d = __builtin_fabsf(nx * rx + nz * rz) - r;
    if (d > 0.0f) {
        vx = v[0];
        vz = v[2];
        k = __builtin_fabsf(nx * vx + nz * vz);
        if (__builtin_fabsf(k) < 0.001f) {
            d = 0.0f;
        } else {
            d = d / k;
        }
        ex = d * vx;
        ez = d * vz;
        d = 1.0f - d;
        t = dx * (d * vx) + dz * (d * vz);
        v[2] = ez + dz * t;
        v[0] = ex + dx * t;
    } else {
        d = __builtin_fabsf(d);
        vx = v[0];
        vz = v[2];
        t = vx * dx + vz * dz;
        v[2] = dz * t + nz * d;
        v[0] = dx * t + nx * d;
    }
    return 2;
}
