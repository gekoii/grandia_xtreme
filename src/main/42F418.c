#include "common.h"
#include "gph.h"
#include "main/n03.h"
#include "protos/main.h"


void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s64);
void MemSet(void *, s32, s32);

void GphViewInitAll(void) {
    s32 i;
    GphView *p;

    MemSet(gphViews, 0, 0xA00);
    MemSet(gphViewGsState, 0, 0x90);
    GsRenderStatePacketSet(gphViewGsState, 1, 1, 1, 1, 1, 0, 0,
                  0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0x20, 0);
    p = gphViews;
    for (i = 0; i < 8; i++) {
        p->unkFC = 16777215.0f;
        p->magH = 1.0f;
        p->magV = 0.9f;
        p++;
        GphViewSetup(i, 0, 0, 0x280, 0x1C0, 8000.0f, 1.0f, 55.0f);
        GphViewSetFog(i, 0xFF, 0xFF, 0xFF, 10000.0f, 255.99899291992188f, 0.0f);
    }
}
