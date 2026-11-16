/* Port-only C for VuCapsuleTriHit (0x0043F1C8), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "vu0_port.h"

f32 VuVecDot(f32 *a, f32 *b);
s32 VuCapsuleSphereHit(void *capsule, f32 *sphere);

s32 VuCapsuleTriHit(void *capsule, f32 *tri);

/*
 * The triangle (vertex 0 and two edges, tri[0..2]) as a sphere: centre at
 * the centroid, radius the farthest vertex's distance plus the capsule's
 * radius (+0x24); then VuCapsuleSphereHit.
 */
s32 VuCapsuleTriHit(void *capsule, f32 *tri) {
    FVECTOR c;
    FVECTOR s;
    FVECTOR d;
    f32 r;
    f32 dd;
    s32 i;

    VuCopy4(c, &tri[4]);
    for (i = 0; i < 3; i++) {
        c[i] = (tri[4 + i] + tri[8 + i]) * VuBits(0x3EAAAAAB); /* 1/3 */
    }
    VuCopy4(s, c);
    for (i = 0; i < 3; i++) {
        s[i] = c[i] + tri[i];
    }
    r = VuVecDot(c, c);
    VuCopy4(d, &tri[4]);
    for (i = 0; i < 3; i++) {
        d[i] = tri[4 + i] - c[i];
    }
    dd = VuVecDot(d, d);
    if (r < dd) {
        r = dd;
    }
    VuCopy4(d, &tri[8]);
    for (i = 0; i < 3; i++) {
        d[i] = tri[8 + i] - c[i];
    }
    dd = VuVecDot(d, d);
    if (r < dd) {
        r = dd;
    }
    s[3] = *(f32 *)((u8 *)capsule + 0x24) + VuSqrt(r);
    return VuCapsuleSphereHit(capsule, s);
}
