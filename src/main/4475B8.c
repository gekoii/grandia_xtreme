/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"

extern s32 gphScanlineY;

extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);
extern GphFbSt GsPackST(f32, f32);

void GphFbScanline(void) {
    s32 rw;
    u32 H;
    GphViewArea r2;
    f32 hf;
    s32 ry;
    u32 th;
    GphFbSt st;
    s32 i;
    GphViewArea r;
    f32 v0;
    u32 off;
    f32 dv;
    GphFbPacket *p;
    f32 tv;
    f32 u0;
    f32 v1;
    s32 d;
    s32 ia;
    u32 tw;
    f32 a;
    u32 W;
    u32 n;
    s32 x0;
    s32 rx;
    s32 ya;
    f32 u1;
    s32 rh;
    s32 y0;
    s32 xa;

    th = 0xFF;
    tw = 0xFF;
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
    rx = r2.x;
    ry = r2.y;
    rw = r2.w;
    rh = H;
    x0 -= off;
    r2.h = H;
    for (i = 0, n = 1; i < 11; i++, n <<= 1) {
        if (n >= W) {
            tw = i;
            W = n;
            break;
        }
    }
    for (i = 0, n = 1; i < 11; i++, n <<= 1) {
        if (n >= H) {
            th = i;
            H = n;
            break;
        }
    }
    gphFbTexInfo.w = W;
    gphFbTexInfo.tw = tw;
    gphFbTexInfo.th = th;
    gphFbTexInfo.size = W * H * 4;
    gphFbTexInfo.h = H;
    GsDrawEnvPacketSet(gphFbPacket, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, r.x, r.y, r.w - 1, r.h - 1, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 0, 0, 0, 0,
                  0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1,
                  0, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&gphFbPacket->texLoad.pkt, &gphFbTexInfo, gsDrawFrameAddr, 0, 0, 1, 1, 0, 0, 0, 0, 0);
    gphFbPacket->texLoad.reg.tex0 = (u64)((u32)(gsDrawFrameAddr + y0 * 0x280 + x0) >> 6) + ((u64)10 << 14) + ((u64)gphFbTexInfo.psm << 20) + ((u64)gphFbTexInfo.tw << 26) + ((u64)gphFbTexInfo.th << 30) + ((u64)1 << 34);
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
    u0 = ((f32)rx + 0.5f) / (f32)W;
    v0 = ((f32)ry + 0.5f) / (f32)H;
    u1 = ((f32)(rx + rw) - 0.5f) / (f32)W;
    v1 = ((f32)(ry + rh) - 0.5f) / (f32)H;
    hf = (f32)rh;
    a = hf * 3.0f / 448.0f;
    d = gphScanlineY;
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000080808080ULL;
    dv = (v1 - v0) / hf;
    tv = v0 + dv * (f32)d;
    d += 0x800;
    xa = (rx + 0x800) << 4;
    x0 = rw << 4;
    ya = (ry + d) << 4;
    ia = (s32)a << 4;
    st = GsPackST(u0, tv);
    p = gphFbPacket;
    p->st0 = st;
    p->xyz0 = xa | (ya << 16);
    st = GsPackST(u1, tv + dv);
    p = gphFbPacket;
    p->st1 = st;
    p->xyz1 = (xa + x0) | ((ya + ia) << 16);
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    gphScanlineY += (s32)((hf + hf) / 448.0f);
    if (gphScanlineY >= rh) {
        gphScanlineY = 0;
    }
}
