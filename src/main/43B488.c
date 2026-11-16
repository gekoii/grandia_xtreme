#include "common.h"
#include "gph.h"

f32 ChrPartGetAlpha(void *arg0) {
    GphPart *p = arg0;

    if (((u16)p->gsFlags >> 0xB) & 3) {
        return p->color[0] * 0.5f;
    }
    return p->color[3];
}
