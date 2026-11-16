#include "common.h"
#include "gph.h"

void GphLightVuBlockReset(void *);

void GphLightListInit(u32 n, GphLightList *p) {
    GphLight *e;
    u32 i;

    p->count = n;
    p->capacity = n;
    GphLightVuBlockReset(&p->block);
    e = (GphLight *)(((u32)p + (n + 4) * 4 + 0x1AF) & 0xFFFFFFE0);
    for (i = 0; i < n; i++) {
        e->enable = 0;
        p->lights[i] = e;
        e++;
    }
}
