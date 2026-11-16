#include "common.h"
#include "gph.h"

void ChrModelAddAlphaRaw(GphModel *arg0, f32 arg1) {
    s32 i;
    GphPart *e = arg0->parts;

    for (i = 0; i < arg0->partCount; i++, e++) {
        if ((e->gsFlags >> 11) & 3) {
            e->color[0] += arg1;
            e->color[1] += arg1;
            e->color[2] += arg1;
        } else {
            e->color[3] += arg1;
        }
    }
}
