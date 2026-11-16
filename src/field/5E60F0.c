/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

void VuVecCopy(f32 *, f32 *);
void GphPoly3DDraw(u32, void *, u32, s32);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VADD(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

s32 MapPolyDebugDraw(void)
{
    FVECTOR v[4];
    MapPolyList *t = &mapPolyList;
    s32 n = t->count;
    MapPolyTri *p = t->tris;
    s32 i;

    for (i = 0; i < n; i++) {
        VuVecCopy(v[0], p->v0);
        VuVecCopy(v[1], p->e1);
        VuVecCopy(v[2], p->e2);
        VADD(v[1], p->e1, p->v0);
        VADD(v[2], p->e2, p->v0);
        copyq(v[3], v[0]);
        v[0][1] += 1.0f;
        v[1][1] += 1.0f;
        v[2][1] += 1.0f;
        v[3][1] += 1.0f;
        v[0][3] = 1.0f;
        v[1][3] = 1.0f;
        v[2][3] = 1.0f;
        v[3][3] = 1.0f;
        GphLine3DDraw(4, v, 0xFFFFFF30, 1);
        GphPoly3DDraw(3, v, 0x20A02030, 1);
        p++;
    }
    return n;
}
