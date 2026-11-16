#include "common.h"
#include "gph.h"

void ChrPartSetAlphaRaw(GphPart *arg0, f32 arg1) {
    if ((arg0->gsFlags >> 0xB) & 3) {
        arg0->color[0] = arg1;
        arg0->color[1] = arg1;
        arg0->color[2] = arg1;
        return;
    }
    arg0->color[3] = arg1;
}
