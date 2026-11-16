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

void GphFbCenterView(void) {
    GphViewArea r2;
    GphViewArea r;
    u32 tw;
    u32 th;
    u32 off;
    u32 W;
    u32 H;
    u32 n;
    s32 i;
    s32 x0;
    s32 y0;
    f32 u0;
    f32 u1;
    f32 v1;
    f32 v0;

    th = 0xFF;
    tw = 0xFF;
    GphFbSwapDrawZBuf();
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
    v0 = 0.0f;
    u0 = (f32)off / (f32)W;
    u1 = ((f32)(off + r2.w) - 1.0f) / ((f32)W - 1.0f);
    v1 = ((f32)r2.h - 1.0f) / ((f32)H - 1.0f);
    gphFbTexInfo.w = W;
    gphFbTexInfo.tw = tw;
    gphFbTexInfo.th = th;
    gphFbTexInfo.size = W * H * 4;
    gphFbTexInfo.h = H;
    GsDrawEnvPacketSet(gphFbPacket, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, r.x, r.y, r.w - 1, r.h - 1, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 2, 0, 2,
                  0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState2, 0, 0, 0, 1, 0, 0, 0,
                  0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 2,
                  65, 0, 0, 0, 1, 1, 0, 0, 0);
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
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000080505050ULL;
    gphFbPacket->st0 = GsPackST(u0, v0);
    gphFbPacket->xyz0 = 0x80008000;
    gphFbPacket->st1 = GsPackST(u1, v1);
    gphFbPacket->xyz1 = 0x9C00A800;
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000080808080ULL;
    gphFbPacket->st0 = GsPackST(u0, v0);
    gphFbPacket->xyz0 = 0x87008A00;
    gphFbPacket->st1 = GsPackST(u1, v1);
    gphFbPacket->xyz1 = 0x95009E00;
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    GsRenderStatePacketSend(gphFbPacket->renderState2);
    GsFromSprDmaWait();
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000050000000ULL;
    gphFbPacket->st0 = GsPackST(u0, v0);
    gphFbPacket->xyz0 = 0x87809E00;
    gphFbPacket->st1 = GsPackST(u1, v1);
    gphFbPacket->xyz1 = 0x95809E80;
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
    gphFbPacket->prim = 6;
    gphFbPacket->rgbaq = 0x3F80000050000000ULL;
    gphFbPacket->st0 = GsPackST(u0, v0);
    gphFbPacket->xyz0 = 0x95008A80;
    gphFbPacket->st1 = GsPackST(u1, v1);
    gphFbPacket->xyz1 = 0x95809E00;
    GsGifPacketSend(&gphFbPacket->dmaTag);
    GsFromSprDmaWait();
}
