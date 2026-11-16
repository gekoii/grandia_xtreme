/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"

extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);
/* u64 for the strip stores below; the two sprite ST stores call it through a
 * GphFbSt cast (as 442200 declares it). */
extern u64 GsPackST(f32, f32);

void GphFbMeshWarp(s32 px, s32 py, s32 pw, s32 ph, f32 k) {
    GphViewArea r2;
    GphViewArea r;
    u32 tw;
    u32 th;
    u32 W;
    s32 j;
    s32 dx;
    s32 dy;
    u32 off;
    u32 H;
    s32 i;
    u32 n;
    s32 x0;
    s32 y0;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    f32 du;
    f32 dv;
    s32 sx;
    s32 sy;
    s32 sw;
    s32 sh;
    s32 fx;
    s32 fy;

    tw = 0xFF;
    GphFbWarpMeshStep(k);
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
                  0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1,
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
    gphFbPacket->st0 = ((GphFbSt (*)(f32, f32))GsPackST)(u0, v0);
    gphFbPacket->xyz0 = sx | (sy << 16);
    gphFbPacket->st1 = ((GphFbSt (*)(f32, f32))GsPackST)(u1, v1);
    gphFbPacket->xyz1 = (sx + sw - 8) | ((sy + sh - 8) << 16);
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    dx = (pw / 8) << 4;
    dy = (ph / 8) << 4;
    ((GphFbStripPacket *)gphFbPacket)->dmaTag = 0x10000009;
    ((GphFbStripPacket *)gphFbPacket)->gifRegs = 0xF5215215215210ULL;
    ((GphFbStripPacket *)gphFbPacket)->vif[3] = 0x50000008;
    ((GphFbStripPacket *)gphFbPacket)->gifTag = (0xE4000000ULL << 32) | 0x8001;
    ((GphFbStripPacket *)gphFbPacket)->prim = 4;
    ((GphFbStripPacket *)gphFbPacket)->dmaTagHi = 0;
    ((GphFbStripPacket *)gphFbPacket)->vif[0] = 0;
    ((GphFbStripPacket *)gphFbPacket)->vif[1] = 0;
    ((GphFbStripPacket *)gphFbPacket)->vif[2] = 0;
    ((GphFbStripPacket *)gphFbPacket)->v[0].rgbaq = 0x3F80000080808080ULL;
    ((GphFbStripPacket *)gphFbPacket)->v[1].rgbaq = 0x3F80000080808080ULL;
    ((GphFbStripPacket *)gphFbPacket)->v[2].rgbaq = 0x3F80000080808080ULL;
    ((GphFbStripPacket *)gphFbPacket)->v[3].rgbaq = 0x3F80000080808080ULL;
    ((GphFbStripPacket *)gphFbPacket)->end = 0;
    fy = (py + 0x800) << 4;
    u0 = ((f32)px + 0.5f) / (f32)W;
    v0 = ((f32)py + 0.5f) / (f32)H;
    u1 = ((f32)(px + pw) - 0.5f) / (f32)W;
    v1 = ((f32)(py + ph) - 0.5f) / (f32)H;
    du = (f32)pw / (f32)W * 0.125f;
    dv = (f32)ph / (f32)H * 0.125f;
    v1 = v0 + dv - 1.0f / (f32)H;
    for (i = 0; i < 8; i++) {
        fx = (px + 0x800) << 4;
        u0 = ((f32)px + 0.5f) / (f32)W;
        u1 = u0 + du - 1.0f / (f32)W;
        for (j = 0; j < 8; j++) {
            ((GphFbStripPacket *)gphFbPacket)->v[0].st = GsPackST(u0 + gphWarpMeshX[i][j], v0 + gphWarpMeshY[i][j]);
            ((GphFbStripPacket *)gphFbPacket)->v[0].xyz = fx | (fy << 16);
            ((GphFbStripPacket *)gphFbPacket)->v[1].st = GsPackST(u1 + gphWarpMeshX[i][j + 1], v0 + gphWarpMeshY[i][j + 1]);
            ((GphFbStripPacket *)gphFbPacket)->v[1].xyz = (fx + dx - 8) | (fy << 16);
            ((GphFbStripPacket *)gphFbPacket)->v[2].st = GsPackST(u0 + gphWarpMeshX[i + 1][j], v1 + gphWarpMeshY[i + 1][j]);
            ((GphFbStripPacket *)gphFbPacket)->v[2].xyz = fx | ((fy + dy - 8) << 16);
            ((GphFbStripPacket *)gphFbPacket)->v[3].st = GsPackST(u1 + gphWarpMeshX[i + 1][j + 1], v1 + gphWarpMeshY[i + 1][j + 1]);
            ((GphFbStripPacket *)gphFbPacket)->v[3].xyz = (fx + dx - 8) | ((fy + dy - 8) << 16);
            GsGifPacketSend(&((GphFbStripPacket *)gphFbPacket)->dmaTag);
            GsFromSprDmaWait();
            u0 += du;
            u1 += du;
            fx += dx;
        }
        fy += dy;
        v0 += dv;
        v1 += dv;
    }
}
