/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n13.h"
#include "field/n17.h"

void MapBorderAddCircle(MapWall *a, void *p, f32 r)
{
    FVECTOR d;
    MapBorderHit *e;
    f32 t;

    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(p), "r"(&a->cx) : "memory");
    t = a->radius + r;
    if (d[0] * d[0] + d[2] * d[2] < t * t) {
        e = &mapBorderHitList->hits[mapBorderHitList->count];
        e->type = 0;
        e->shape = a;
        mapBorderHitList->count++;
    }
}
