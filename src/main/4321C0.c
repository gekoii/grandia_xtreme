/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphLightVuBlockReset(void *);
void VuLightTransform(void *, s32);
void FlushCache(s32);

void GphLightListUpdate(void) {
    u32 cnt;
    GphLight *e;
    FVECTOR *d1;
    FVECTOR *d2;
    s32 i;
    u32 n;
    FVECTOR *d2b;
    s32 sp0;
    FVECTOR *d1b;
    GphLightVuBlock *fp;
    GphLightList *base;

    base = gphLightList;
    fp = &base->block;
    GphLightVuBlockReset(fp);
    d1b = base->block.pointColor;
    d2b = base->block.pointPos;
    cnt = 0;
    d1 = base->block.dirColor;
    d2 = base->block.dirVec;
    n = gphLightList->count;
    sp0 = VuMtxStackGetTop();
    copyq((void *)0x70002740, GphLightGet(0)->color);
    for (i = 1; i < n; i++) {
        e = GphLightGet(i);
        if (e->type == 0) {
            if (e->enable == 1) {
                copyq(d1, e->color);
                copyq(d2, e->dir);
                d1++;
                cnt++;
                (*d2)[3] = e->intensity;
                d2++;
            }
        }
        if (cnt >= 4) break;
    }
    d1 = d1b;
    d2 = d2b;
    for (i = 1, cnt = 0; cnt < 4 && i < n; i++) {
        e = GphLightGet(i);
        if (e->type == 1) {
            if (e->enable == 1) {
                (*d1)[0] = e->color[0] * e->intensity;
                (*d1)[1] = e->color[1] * e->intensity;
                (*d1)[2] = e->color[2] * e->intensity;
                copyq(d2, e->dir);
                d1++;
                cnt++;
                (*d2)[3] = 1.0f / e->range;
                d2++;
            }
        }
    }
    VuLightTransform(fp, sp0);
    FlushCache(0);
}
