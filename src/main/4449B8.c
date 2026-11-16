/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"

extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);
extern GphFbSt GsPackST(f32, f32);

void GphFbMultiScreen(s32 px, s32 py, s32 pw, s32 ph, s32 nx, s32 ny) {
    GphViewArea r2;
    GphViewArea r;
    u32 tw;
    u32 th;
    u32 off;
    u32 W;
    u32 H;
    s32 i;
    u32 n;
    s32 j;
    s32 k;
    s32 x0;
    s32 y0;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    f32 dx;
    f32 dy;
    f32 fx;
    f32 fy;
    s32 sx;
    s32 sy;
    s32 sw;
    s32 sh;

    tw = 0xFF;
    th = 0xFF;
    GphFbSwapDrawZBuf();
    GphViewGetArea(0, &r.x);
    x0 = r.x;
    y0 = r.y;
    r2.x = x0;
    r2.y = y0;
    W = r.w;
    r2.w = W;
    off = x0 & 0x3F;
    W += off;
    H = r.h;
    r2.h = H;
    x0 -= off;
    dx = (f32)pw / (f32)nx;
    dy = (f32)ph / (f32)ny;
    for (n = 1, i = 0; i < 11; i++, n <<= 1) {
        if (n >= W) {
            tw = i;
            W = n;
            break;
        }
    }
    for (n = 1, i = 0; i < 11; i++, n <<= 1) {
        if (n >= H) {
            th = i;
            H = n;
            break;
        }
    }
    u0 = 0.5f / (f32)W;
    v0 = 0.5f / (f32)H;
    u1 = ((f32)r2.w - 0.5f) / (f32)W;
    v1 = ((f32)r2.h - 0.5f) / (f32)H;
    gphFbTexInfo.w = W;
    gphFbTexInfo.tw = tw;
    gphFbTexInfo.th = th;
    gphFbTexInfo.size = W * H * 4;
    gphFbTexInfo.h = H;
    GsDrawEnvPacketSet(gphFbPacket, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, r.x, r.y, r.w - 1, r.h - 1, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 0, 0, 0, 0,
                  0, 1, 1, 0x80, 0, 0, 0, 1, 1, 0, 1, 0, 1,
                  0, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&gphFbPacket->texLoad.pkt, &gphFbTexInfo, gsZBufAddr, 0, 0, 1, 1, 0, 0, 0, 0, 0);
    gphFbPacket->texLoad.reg.tex0 = (u64)((u32)(gsZBufAddr + y0 * 0x280 + x0) >> 6) + ((u64)10 << 14) + ((u64)gphFbTexInfo.psm << 20) + ((u64)gphFbTexInfo.tw << 26) + ((u64)gphFbTexInfo.th << 30) + ((u64)1 << 34);
    gphFbPacket->dmaTag = 0x10000005;
    gphFbPacket->gifRegs = 0x424210;
    gphFbPacket->vif[3] = 0x50000004;
    gphFbPacket->gifTag = (0x64000000ULL << 32) | 0x8001;
    gphFbPacket->dmaTagHi = 0;
    gphFbPacket->vif[0] = 0;
    gphFbPacket->vif[1] = 0;
    gphFbPacket->vif[2] = 0;
    GsDrawEnvPacketSend(gphFbPacket);
    GsRenderStatePacketSend(gphFbPacket->renderState);
    GsTexPacketSend(&gphFbPacket->texLoad.pkt);
    sx = (r.x + 0x800) << 4;
    sy = (r.y + 0x800) << 4;
    sw = r.w << 4;
    sh = r.h << 4;
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000080808080ULL;
    gphFbPacket->st0 = GsPackST(u0, v0);
    gphFbPacket->xyz0 = sx | (sy << 16);
    gphFbPacket->st1 = GsPackST(u1, v1);
    gphFbPacket->xyz1 = (sx + sw - 8) | ((sy + sh - 8) << 16);
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    u0 = ((f32)px + 0.5f) / (f32)W;
    v0 = ((f32)py + 0.5f) / (f32)H;
    u1 = ((f32)(px + pw) - 0.5f) / (f32)W;
    v1 = ((f32)(py + ph) - 0.5f) / (f32)H;
    fy = py;
    for (j = 0; j < ny; j++) {
        fx = px;
        for (k = 0; k < nx; k++) {
            gphFbPacket->prim = 6;
            gphFbPacket->rgbaq = 0x3F80000080808080ULL;
            gphFbPacket->st0 = GsPackST(u0, v0);
            gphFbPacket->xyz0 = (((s32)fx << 4) + 0x7FF8) | ((((s32)fy << 4) + 0x7FF8) << 16);
            gphFbPacket->st1 = GsPackST(u1, v1);
            gphFbPacket->xyz1 = (((s32)ceilf(fx + dx) << 4) + 0x7FF8) | ((((s32)ceilf(fy + dy) << 4) + 0x7FF8) << 16);
            fx += dx;
            GsGifPacketSend(&gphFbPacket->dmaTag);
            GsFromSprDmaWait();
        }
        fy += dy;
    }
}
