/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n13.h"
#include "field/n17.h"

typedef struct {
    u8 pad[0x20];
    VuVec4 vel;
    VuVec4 push;
} Mover;

s32 MapBorderSweepWall(MapBorderWall *, Mover *);
s32 MapBorderSweepCircle(f32 *, Mover *, f32);
void VuVecCopy(VuVec4 *, VuVec4 *);
f32 VuVecDot(VuVec4 *, VuVec4 *);
static inline f32 sq(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 MapBorderSweep(Mover *arg0) {
    VuVec4 best;
    VuVec4 *v;
    s32 i;
    s32 n;
    s32 hits;
    s32 r;
    f32 d;
    f32 min;
    MapBorderHit *e;
    MapWall *s;

    if (mapBorderHitList->count == 0) {
        return 0;
    }
    v = &arg0->vel;
    if (v->x == 0.0f && v->z == 0.0f) {
        return 0;
    }
    d = VuVecDot(v, v);
    VuVecCopy(&best, v);
    min = sq(d);
    i = 0;
    n = mapBorderHitList->count;
    hits = 0;
    for (; i < n; i++) {
        e = &mapBorderHitList->hits[i];
        if (e->type != 0) {
            r = MapBorderSweepWall(&e->wall, arg0);
        } else {
            s = e->shape;
            r = MapBorderSweepCircle(&s->cx, arg0, s->radius);
        }
        if (r != 0) {
            d = VuVecDot(&arg0->push, &arg0->push);
            hits++;
            d = sq(d);
            if (d < min) {
                min = d;
                VuVecCopy(&best, &arg0->push);
            }
        }
    }
    VuVecCopy(&arg0->push, &best);
    return hits > 0;
}
