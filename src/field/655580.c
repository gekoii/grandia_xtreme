#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void GphModelDeformNearPoint(void *, void *);

void WmapDeformTerrain(void) {
    GphModel *o;

    if (wmapWork->active != 0) {
        o = MapModelGetObj(0);
        if (o != NULL) {
            GphModelDeformWaveLit(o, wmapWork->waveT, 32.0f, 0.004f);
            o = MapModelGetObj(4);
            if (o != NULL) {
                copyq(&wmapWork->markerPos, &o->pos);
                /* s32 cast: the call stays jal (no tail call) */
                ((s32 (*)(void *, void *))GphModelDeformNearPoint)(MapModelGetObj(0), &wmapWork->markerPos);
            }
        }
    }
}
