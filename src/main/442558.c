/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x004A6898-0x004A68A4 */
#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"


extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);
extern GphFbSt GsPackST(f32, f32);

void GphFbBlur(s32 px, s32 py, s32 pw, s32 ph, u8 ci, u8 alpha) {
    u8 cols[3][4] = { { 0, 1, 2, 1 }, { 0, 2, 2, 1 }, { 0, 0, 0, 0 } };
    GphViewArea r;
    GphViewArea r2;
    u32 tw;
    u32 th;
    u32 off;
    u32 W;
    u32 H;
    s32 i;
    u32 n;
    s32 x0;
    s32 y0;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    GphFbSt st;
    GphFbPacket *p;
    u64 xyz;
    s64 ofs;
    s32 sx;
    s32 sy;

    tw = 0xFF;
    th = 0xFF;
    GphViewGetArea(0, &r.x);
    x0 = r.x;
    y0 = r.y;
    r2.x = x0;
    r2.y = y0;
    r2.w = r.w;
    off = x0 & 0x3F;
    W = r.w + off;
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
    u0 = ((f32)px + 0.5f) / (f32)W;
    v0 = ((f32)py + 0.5f) / (f32)H;
    u1 = ((f32)(px + pw) - 0.5f) / (f32)W;
    v1 = ((f32)(py + ph) - 0.5f) / (f32)H;
    gphFbTexInfo.w = W;
    gphFbTexInfo.tw = tw;
    gphFbTexInfo.th = th;
    gphFbTexInfo.size = W * H * 4;
    gphFbTexInfo.h = H;
    GsDrawEnvPacketSet(gphFbPacket, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, r.x, r.y, r.w - 1, r.h - 1, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 1, 0, 0, 0,
                  0, 0, 5, 0x40, 1, 0, 0, 1, 1, cols[ci][0], cols[ci][1], cols[ci][2], cols[ci][3],
                  alpha, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&gphFbPacket->texLoad.pkt, &gphFbTexInfo, gsDispFrameAddr, 0, 0, 1, 1, 0, 0, 0, 0, 0);
    gphFbPacket->texLoad.reg.tex0 = (u64)((u32)(gsDispFrameAddr + y0 * 0x280 + x0) >> 6) + ((u64)10 << 14) + ((u64)gphFbTexInfo.psm << 20) + ((u64)gphFbTexInfo.tw << 26) + ((u64)gphFbTexInfo.th << 30) + ((u64)1 << 34);
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
    ofs = 0x8000;
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000080808080ULL;
    st = GsPackST(u0, v0);
    p = gphFbPacket;
    p->st0 = st;
    xyz = __fixsfdi((f32)px * 16.0f) + ofs;
    xyz &= 0xFFFF;
    xyz |= ((__fixsfdi((f32)py * 16.0f) + ofs) & 0xFFFF) << 16;
    p->xyz0 = xyz;
    gphFbPacket->st1 = GsPackST(u1, v1);
    sx = pw + 0x800;
    sy = ph + 0x800;
    gphFbPacket->xyz1 = (((px + sx) << 4) - 8) | ((((py + sy) << 4) - 8) << 16);
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
}
