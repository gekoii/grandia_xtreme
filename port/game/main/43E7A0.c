/* Port-only C for VuSphereTriBoundHit (0x0043E7A0), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "vu0_port.h"

f32 VuVecDot(f32 *a, f32 *b);
f32 VuVecNormalizeLen(f32 *dst, f32 *src);

s32 VuSphereTriBoundHit(f32 *pos, f32 radius, f32 *tri);

/*
 * Sphere (pos, radius) against the bounding sphere of a triangle given as
 * vertex 0 and two edges (tri[0], tri[1], tri[2]): 1 when the sphere's
 * centre is within radius of the triangle's centroid, or nearer to it than
 * radius plus the distance of some vertex.
 */
s32 VuSphereTriBoundHit(f32 *pos, f32 radius, f32 *tri) {
    FVECTOR c;
    FVECTOR d;
    f32 len;
    s32 i;

    for (i = 0; i < 4; i++) {
        c[i] = (tri[4 + i] + tri[8 + i]) * VuBits(0x3EAAAAAB); /* 1/3 */
    }
    for (i = 0; i < 4; i++) {
        d[i] = (pos[i] - tri[i]) - c[i];
    }
    len = VuVecNormalizeLen(d, d);
    if (len < radius) {
        return 1;
    }
    len = len - radius;
    len = len * len;
    if (len < VuVecDot(c, c)) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        d[i] = tri[4 + i] - c[i];
    }
    if (len < VuVecDot(d, d)) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        d[i] = tri[8 + i] - c[i];
    }
    if (len < VuVecDot(d, d)) {
        return 1;
    }
    return 0;
}
