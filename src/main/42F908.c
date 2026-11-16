#include "common.h"
#include "gph.h"
#include "main/n03.h"
#include "protos/main.h"


void VuViewClipSetup(void *, GphView *, void *, f32 *, f32);
s32 VuFloatToInt(f32);
void GphViewLoadScratch(void *);

void GphViewSelect(s32 idx, f32 angle) {
    s32 i;
    GphView *e;
    s32 a;

    GsRenderStatePacketSend(gphViewGsState);
    for (i = 0; i < 8; i++) {
        VuViewClipSetup((u8 *)0x70000400 + i * 0x80, &gphViews[i], gphViews[i].fogPkt, &gphViews[i].x, 100.0f);
    }
    e = &gphViews[idx];
    GsDrawEnvPacketSend(e);
    GsFogPacketSend(gphViews[idx].fogPkt);
    a = VuFloatToInt(e->x);
    DbgTextSetOffset(a, VuFloatToInt(e->y));
    GphViewLoadScratch((u8 *)0x70000400 + idx * 0x80);
    VuViewRollSet((s32)(-angle * 10430.378f));
}
