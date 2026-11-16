#include "common.h"
#include "gph.h"

f32 ChrPartGetAlphaRaw(GphPart *p) {
    if (((u16)p->gsFlags >> 0xB) & 3) {
        return p->color[0];
    }
    return p->color[3];
}
