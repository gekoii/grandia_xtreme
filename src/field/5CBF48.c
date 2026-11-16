#include "common.h"
#include "protos/field.h"
#include "field/n15.h"
#include "field/n17.h"

void VuVecCopy(void *, void *);

void CamDebugTriAdd(MapPolyTri *src)
{
    MapPolyList *l;
    f32 *o;
    FVECTOR (*t)[3];

    l = MapPolyGetList();
    /* through a row pointer: indexing camDebugTris->verts directly is 1 word off */
    t = camDebugTris->verts;
    o = t[l->count][0];
    VuVecCopy(o, src->v0);
    o[1] = o[1] + 10.0f;
    o[3] = 1.0f;
    o += 4;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(o), "r"(src->e1), "r"(src->v0) : "memory");
    o[1] = o[1] + 10.0f;
    o[3] = 1.0f;
    o += 4;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(o), "r"(src->e2), "r"(src->v0) : "memory");
    o[1] = o[1] + 10.0f;
    o[3] = 1.0f;
    camDebugTris->count++;
}
