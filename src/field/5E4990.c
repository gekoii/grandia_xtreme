/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n13.h"
#include "field/n17.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MapBorderAddPolygon(MapWall *a, VuVec3 *p, f32 rad) {
    FVECTOR *prev;
    FVECTOR *q;
    s32 i;
    f32 len;
    f32 ex, ez, px, pz;
    f32 t;
    f32 d;
    MapBorderHit *e;
    MapBorderWall *w;
    f32 *v;
    f32 qx, qz;

    prev = &a->points[0];
    v = (f32 *)p;
    {
        f32 z2, cx, cz;
        cx = a->cx;
        cz = a->unk14; /* the game reads +0x14 (0), not the centre z at +0x18 */
        qx = v[0];
        qz = v[2];
        z2 = qz - cz;
        qx -= cx;
        cx = qx * qx;
        z2 = cx + z2 * z2;
        len = fsqrt(z2);
    }
    if (a->radius + rad < len) {
        return;
    }
    for (i = 1; i < a->count; i++) {
        q = prev;
        prev = &a->points[i];
        qx = (*q)[0];
        qz = (*q)[2];
        v = (f32 *)p;
        ex = (*prev)[0];
        ez = (*prev)[2];
        px = *v;
        v += 2;
        pz = *v;
        px -= qx;
        ex -= qx;
        pz -= qz;
        ez -= qz;
        if (ex * ex + ez * ez < 1.0f) {
            continue;
        }
        len = fsqrt(ex * ex + ez * ez);
        qz = 1.0f / len;
        qx = -ez;
        qx = __builtin_fabsf((qx * px + ex * pz) * qz);
        if (qx <= rad) {
            t = (px * ex + pz * ez) * qz;
            if (len < t) {
                px -= ex;
                pz -= ez;
                d = fsqrt(px * px + pz * pz);
                if (rad < d) {
                    continue;
                }
                qx = __builtin_fabsf(d);
            } else if (t < 0.0f) {
                d = fsqrt(px * px + pz * pz);
                if (rad < d) {
                    continue;
                }
                qx = __builtin_fabsf(d);
            }
            e = &mapBorderHitList->hits[mapBorderHitList->count];
            w = &e->wall;
            e->type = 1;
            e->dist = qx;
            w->p0 = q;
            w->len = len;
            mapBorderHitList->count++;
            w->dx = ex * qz;
            w->dz = ez * qz;
            w->p1 = prev;
        }
    }
}
