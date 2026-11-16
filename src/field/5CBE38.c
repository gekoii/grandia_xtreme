#include "common.h"
#include "protos/field.h"
#include "field/n15.h"
#include "field/n17.h"

void VuVecCopy(void *, void *);

void CamDebugTriAddAll(void)
{
    MapPolyList *l;
    s32 i;
    s32 n;
    MapPolyTri *e;
    f32 *o;

    i = 0;
    l = MapPolyGetList();
    n = l->count;
    e = l->tris;
    o = camDebugTris->verts[0][0];
    for (; i < n; i++) {
        VuVecCopy(o, e->v0);
        o[1] = o[1] + 0.0f;
        o[3] = 1.0f;
        o += 4;
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(o), "r"(e->v0), "r"(e->e1) : "memory");
        o[1] = o[1] + 0.0f;
        o[3] = 1.0f;
        o += 4;
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(o), "r"(e->v0), "r"(e->e2) : "memory");
        o[1] = o[1] + 0.0f;
        o[3] = 1.0f;
        o += 4;
        e++;
        if (i >= 0x80) {
            break;
        }
    }
    camDebugTris->count = i;
}
