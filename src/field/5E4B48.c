/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

typedef struct {
    s32 type;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
} Area;

typedef struct {
    f32 *p0;
    f32 *p1;
    u32 area;
} Seg;

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void MapBorderAddSegments(Seg *segs, f32 *pos, s32 n, f32 r)
{
    FVECTOR d;
    FVECTOR s;
    s32 i;
    f32 *a;
    f32 *b;
    Area *ar;
    f32 y;
    f32 top;
    f32 len;
    f32 len2;
    f32 inv;
    f32 dist;
    f32 proj;
    f32 f;
    MapBorderHit *c;
    MapBorderWall *w;

    for (i = 0; i < n; i++) {
        a = segs[i].p0;
        b = segs[i].p1;
        if (segs[i].area != 0xFFFFFFFF) {
            ar = MapWallGet(segs[i].area);
            if (ar->unk8 != 0) {
                top = ar->unkE;
                y = pos[1];
                if (y + 60.0f < top) {
                    continue;
                }
                if ((f32)ar->unkC < y) {
                    continue;
                }
            }
        }
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(d), "r"(pos), "r"(a) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(s), "r"(b), "r"(a) : "memory");
        len2 = s[0] * s[0] + s[2] * s[2];
        if (len2 < 1.0f) {
            continue;
        }
        len = fsqrt(len2);
        inv = 1.0f / len;
        dist = __builtin_fabsf((-s[2] * d[0] + s[0] * d[2]) * inv);
        if (!(dist <= r)) {
            continue;
        }
        proj = (d[0] * s[0] + d[2] * s[2]) * inv;
        if (len < proj) {
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf2, 0(%2)\n\t"
                "vsub.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(d), "r"(d), "r"(s) : "memory");
            f = fsqrt(d[0] * d[0] + d[2] * d[2]);
            if (r < f) {
                continue;
            }
            dist = __builtin_fabsf(f);
        } else if (proj < 0.0f) {
            f = fsqrt(d[0] * d[0] + d[2] * d[2]);
            if (r < f) {
                continue;
            }
            dist = __builtin_fabsf(f);
        }
        c = &mapBorderHitList->hits[mapBorderHitList->count];
        w = &c->wall;
        c->type = 1;
        c->dist = dist;
        w->p0 = (FVECTOR *)a;
        w->p1 = (FVECTOR *)b;
        w->dx = s[0] * inv;
        w->dz = s[2] * inv;
        w->len = len;
        mapBorderHitList->count++;
    }
}
