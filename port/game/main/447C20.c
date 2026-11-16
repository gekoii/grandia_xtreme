/* Port-only C for GphFbRippleDraw (0x00447C20), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

/* As src/main/442200.c declares them */
extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);

extern void *gphRippleGrid;   /* 33 x 33 vertices of 0x20 bytes (GphFbRippleBuild 448068) */
extern void *gphRipplePacket;

void GphFbRippleDraw(void);

/*
 * Draws the frame buffer (copied into the Z buffer area, which becomes the
 * texture) through the ripple grid: the same draw environment, render
 * state and texture setup as GphFbFlipCopy (442200), then one GIF packet
 * per grid row, a triangle strip of 66 vertices (rows r and r + 1).
 *
 * A grid vertex is a position quadword (x, y, z, w) and s, t, RGBA, unused;
 * each strip vertex is ST, RGBAQ (q = 1), XYZ2 ((x, y) + 2048 and z in
 * 12.4 fixed point, vftoi4).
 */
void GphFbRippleDraw(void) {
    s32 area[4];
    u32 x;
    u32 y;
    u32 off;
    u32 W;
    u32 H;
    u32 n;
    u32 tw = 0xFF;
    u32 th = 0xFF;
    s32 i;
    s32 row;
    u8 *pkt;

    GphViewGetArea(0, area);
    x = area[0];
    y = area[1];
    off = x & 0x3F;
    x -= off;
    W = area[2] + off;
    H = area[3];
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
    gphFbTexInfo.w = W;
    gphFbTexInfo.tw = tw;
    gphFbTexInfo.th = th;
    gphFbTexInfo.h = H;
    gphFbTexInfo.size = W * H * 4;
    GphFbSwapDrawZBuf();
    GsDrawEnvPacketSet(gphFbPacket, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, 0, 0, 0x27F, 0x1BF, 0, 0, 0);
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 0, 0, 0, 0,
                  0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1,
                  0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsTexLoadPacket(&gphFbPacket->texLoad.pkt, &gphFbTexInfo, gsZBufAddr, 0, 0, 1, 1, 0, 0, 0, 0, 0);
    gphFbPacket->texLoad.reg.tex0 = (u64)((u32)(gsZBufAddr + y * 0x280 + x) >> 6) + ((u64)1 << 34) +
                                    ((u64)gphFbTexInfo.tw << 26) + ((u64)gphFbTexInfo.psm << 20) +
                                    ((u64)gphFbTexInfo.th << 30) + ((u64)10 << 14);
    GsDrawEnvPacketSend((u8 *)gphFbPacket);
    GsRenderStatePacketSend(gphFbPacket->renderState);
    GsTexPacketSend(&gphFbPacket->texLoad.pkt);

    pkt = gphRipplePacket;
    for (row = 0; row < 32; row++) {
        u8 *o;
        s32 col;

        *(u64 *)(pkt + 0x00) = 0x10000067;          /* DMA cnt, 0x67 qwords */
        *(u64 *)(pkt + 0x08) = 0;
        *(u32 *)(pkt + 0x10) = 0;
        *(u32 *)(pkt + 0x14) = 0;
        *(u32 *)(pkt + 0x18) = 0;
        *(u32 *)(pkt + 0x1C) = 0x50000066;          /* VIF DIRECT 0x66 */
        *(u64 *)(pkt + 0x20) = (0x14000000ULL << 32) | 0x8001; /* GIF A+D, 1 */
        *(u64 *)(pkt + 0x28) = 0;
        *(u64 *)(pkt + 0x30) = 4;                   /* PRIM: triangle strip */
        *(u64 *)(pkt + 0x38) = 0;
        *(u64 *)(pkt + 0x40) = (0x34000000ULL << 32) | 0x8042; /* GIF reglist, 66 */
        *(u64 *)(pkt + 0x48) = 0x512;               /* ST, RGBAQ, XYZ2 */
        o = pkt + 0x50;
        for (col = 0; col < 33; col++) {
            s32 k;

            for (k = 0; k < 2; k++) {
                u8 *g = (u8 *)gphRippleGrid + (row + k) * 0x420 + col * 0x20;
                f32 p[4];
                f32 q = 1.0f;
                s32 ix;
                s32 iy;
                s32 iz;

                VuCopy4(p, g);
                p[0] = p[0] + VuBits(0x45000000); /* 2048 */
                p[1] = p[1] + VuBits(0x45000000);
                ix = VuFtoi(p[0], 4);
                iy = VuFtoi(p[1], 4);
                iz = VuFtoi(p[2], 4);
                __builtin_memcpy(o + 0x0, g + 0x10, 8);
                __builtin_memcpy(o + 0x8, g + 0x18, 4);
                __builtin_memcpy(o + 0xC, &q, 4);
                *(u32 *)(o + 0x10) = (u16)ix | ((u32)(u16)iy << 16);
                *(s32 *)(o + 0x14) = iz;
                o += 0x18;
            }
        }
        GsGifPacketSend((u16 *)pkt);
        GsFromSprDmaWait();
        pkt = o;
    }
}
