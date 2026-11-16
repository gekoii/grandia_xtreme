/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

extern s32 mapPolyHitFlags;

f32 VuVecDot(f32 *, f32 *);
/* declared float-before-pointer: the argument loads only match in this order (EABI) */
s32 MapPolyHitTest(f32 *, f32 *, f32, MapPolyTri *);

#define VADD(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyzw $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 MapPolyResolve(f32 *vel, f32 *pos, f32 dy)
{
    FVECTOR t;
    MapPolyList *tbl = &mapPolyList;
    s32 n = tbl->count;
    MapPolyTri *items;
    MapPolyTri *it;
    s32 i;
    f32 lim;
    f32 py;
    f32 y0;
    f32 h;
    f32 dh;
    f32 best;
    f32 top;
    f32 vy;
    s32 found1;
    s32 found2;

    lim = fsqrt(VuVecDot(vel, vel));
    if (lim < 50.1f) {
        lim = 50.1f;
    }
    items = tbl->tris;
    y0 = pos[1];
    VADD(t, pos, vel);
    for (i = 0; i < n; i++) {
        it = &items[i];
        if (__builtin_fabsf(it->normal[1]) >= 0.7071068f) {
            it->kind = 0;
        } else {
            h = 0.0f;
            dh = it->e1[1];
            if (h < dh) {
                h = dh;
            }
            dh = it->e2[1];
            if (h < dh) {
                h = dh;
            }
            dh = h + it->v0[1] - y0;
            if (dh <= 50.1f) {
                it->kind = 1;
            } else {
                it->kind = 2;
            }
        }
    }
    pos[1] += dy;
    for (i = 0; i < n; i++) {
        it = &items[i];
        if (it->kind == 2 && MapPolyHitTest(pos, vel, dy, it) && MapPolyWallSlide(pos, vel, it, dy)) {
            mapPolyHitFlags |= 2;
        }
    }
    found1 = 0;
    found2 = 0;
    py = pos[1] - dy;
    pos[1] = py;
    best = py;
    t[1] -= dy;
    vy = vel[1];
    top = py + vy;
    for (i = 0; i < n; i++) {
        it = &items[i];
        if (it->kind == 2) {
            continue;
        }
        if (!MapPolyHitTest(pos, vel, 1.0f, it)) {
            continue;
        }
        h = MapPolyHeightAt(t, it);
        dh = h - py;
        if (__builtin_fabsf(dh) < 0.01f && !found1) {
            best = h;
            found1 = 1;
        } else if (0.0f < dh) {
            if (!(h < best)) {
                if (dh <= lim) {
                    best = h;
                    found1 = 1;
                } else if (MapPolyNearestVertexHeight(t, it) - py <= lim) {
                    best = h;
                    found1 = 1;
                }
            }
        } else if (vy < dh && top < h) {
            top = h;
            found2 = 1;
        }
    }
    if (found1) {
        vel[1] = best - py;
        mapPolyHitFlags |= 1;
        return 0;
    }
    if (found2) {
        vel[1] = top - py;
        mapPolyHitFlags |= 1;
        return 0;
    }
    return 0;
}
