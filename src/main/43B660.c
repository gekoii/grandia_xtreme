#include "common.h"
#include "gph.h"

void ChrModelAddAlpha(GphModel *arg0, f32 f) {
    s32 i;
    GphPart *e;
    f = f + f;
    e = arg0->parts;
    for (i = 0; i < arg0->partCount; i++, e++) {
            if ((e->gsFlags >> 11) & 3) {
                e->color[0] += f;
                e->color[1] += f;
                e->color[2] += f;
            } else {
                e->color[3] += f;
            }
    }
}
