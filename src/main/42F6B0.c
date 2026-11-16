#include "common.h"
#include "gph.h"


void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32);

void GphViewSetArea(s32 i, s32 x, s32 y, s32 w, s32 h) {
    GphView *e;

    if (i < 8) {
        e = &gphViews[i];
        GsDrawEnvPacketSet(e, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 0,
                      x, y, x + w - 1, y + h - 1, 0, 0, 0);
        e->x = x;
        e->y = y;
        e->w = w;
        e->h = h;
        e->centerX = w * 0.5f;
        e->centerY = h * 0.5f;
    }
}
