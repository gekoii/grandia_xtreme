#include "common.h"
#include "field/n17.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void MapPolyAdd(MapPolyTri *arg0) {
    MapPolyTri *p;

    if (mapPolyList.count < 0x100) {
        p = &mapPolyList.tris[mapPolyList.count];
        copyq(p->v0, arg0->v0);
        copyq(p->e1, arg0->e1);
        copyq(p->e2, arg0->e2);
        copyq(p->normal, arg0->normal);
        mapPolyList.count++;
    }
}
