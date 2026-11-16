#include "common.h"
#include "gph.h"
void GsFogPacketSet(u8 *, u8, u8, u8, f32, f32, f32);
void GphViewSetFog(s32 idx, s32 r, s32 g, s32 b, f32 x, f32 y, f32 z) {
    GphView *e = &gphViews[idx];
    e->fogR = r;
    e->fogG = g;
    e->fogB = b;
    e->fogParam[0] = x;
    e->fogParam[1] = y;
    e->fogParam[2] = z;
    GsFogPacketSet(gphViews[idx].fogPkt, e->fogR, e->fogG, e->fogB, e->fogParam[0], e->fogParam[1], e->fogParam[2]);
}
