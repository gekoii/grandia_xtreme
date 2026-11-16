/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "field/n17.h"
#include "protos/field.h"

s32 MapBorderSlideWall(MapBorderWall *w, VuVec4 *v, VuVec4 *pos, f32 r) {
    VuVec4 pt;
    f32 ez;
    f32 t;
    f32 s;
    f32 d;
    f32 ex;
    f32 nz;
    f32 px;
    f32 rx;
    f32 vz;
    f32 dz;
    f32 dx;
    f32 nx;
    f32 pz;
    f32 rz;
    f32 vx;
    f32 a;
    f32 b;
    f32 one;
    dz = w->dz;
    dx = w->dx;
    px = (*w->p0)[0];
    rx = pos->x - px;
    pz = (*w->p0)[2];
    rz = pos->z - pz;
    nx = -dz;
    nz = dx;
    t = rx * nx + rz * nz;
    if (t < 0.0f) {
        nz = -nz;
        nx = dz;
    }
    vx = v->x;
    vz = v->z;
    ex = vx + rx;
    ez = vz + rz;
    t = ex * nx + ez * nz;
    if (r < t) {
        return 0;
    }
    t = w->len;
    s = ex * dx + ez * dz;
    if (t < s || s < 0.0f) {
        pt.x = px + dx * t;
        pt.y = 0.0f;
        pt.z = pz + dz * t;
        return MapCollPushCircle((f32 *)&pt, (f32 *)v, (f32 *)pos, 0.0f, r);
    }
    s = __builtin_fabsf(nx * rx + nz * rz);
    d = s - r;
    do { /* do-while(0) (and the inner one): needed for register allocation */
    if (d > 0.0f) {
        do {
        s = __builtin_fabsf(nx * vx + nz * vz);
        if (__builtin_fabsf(s) < 0.001f) {
            d = 0.0f;
            one = 1.0f;
        } else {
            d = d / s;
            one = 1.0f;
        }
        ez = d * vz;
        } while (0);
        ex = d * vx;
        d = one - d;
        b = d * vx;
        a = d * vz;
        t = dx * b + dz * a;
        b = dx * t;
        a = dz * t;
        v->z = ez + a;
        v->x = ex + b;
    } else {
        d = __builtin_fabsf(d);
        t = vx * dx + vz * dz;
        b = dx * t;
        a = dz * t;
        b = b + nx * d;
        a = a + nz * d;
        v->z = a;
        v->x = b;
    }
    } while (0);
    return 1;
}
