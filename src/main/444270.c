#include "common.h"
#include "gph.h"
#include "protos/main.h"

extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s64);
extern void GsTexUpload(void *);
extern void FlushCache(s32);

void GphFbOverlayImage(s32 x, s32 y, s32 w, u32 h, u8 alpha, s32 addr) {
    u8 buf[0x140];
    s32 done;
    u32 tw;
    u32 th;
    u32 W;
    u32 hh;
    s32 i;
    u32 n;
    s32 fx;
    s32 fy;
    s32 fh;
    s32 fy2;
    s32 a;

    a = alpha;
    if (alpha > 0x80) {
        a = 0x80;
    }
    done = 0;
    tw = 0xFF;
    th = 0xFF;
    GsRenderStatePacketSet(gphFbPacket->renderState, 1, 1, 0, 1, 0, 1, 0, 0, 0, 5, 0x40, 1, 0, 0, 1, 1, 0, 1, 2, 1,
                  a, 0, 0, 0, 0, 0, 0, 0, 0);
    gphFbPacket->dmaTag = 0x10000005;
    gphFbPacket->gifRegs = 0x535310;
    gphFbPacket->vif[3] = 0x50000004;
    gphFbPacket->gifTag = ((u64)0x64000000 << 32) | 0x8001;
    gphFbPacket->dmaTagHi = 0;
    gphFbPacket->vif[0] = 0;
    gphFbPacket->vif[1] = 0;
    gphFbPacket->vif[2] = 0;
    GsRenderStatePacketSend(gphFbPacket->renderState);
    W = w + (x & 0x3F);
    fx = (x + 0x800) << 4;
    fy = (y + 0x800) << 4;
    if (W & 0x3F) {
        W += 0x40 - (W & 0x3F);
    }
    for (n = 1, i = 0; i < 11; i++, n <<= 1) {
        if (n >= W) {
            tw = i;
            break;
        }
    }
    do {
        if (W * h > 0x11800) {
            hh = 0x11800 / W;
            h -= hh;
        } else {
            /* do-while(0): loop weight of hh/done (allocation) */
            do {
                hh = h;
                done = 1;
            } while (0);
        }
        for (n = 1, i = 0; i < 11; i++, n <<= 1) {
            if (n >= hh) {
                th = i;
                break;
            }
        }
        fh = hh << 4;
        gphFbUploadTexInfo.w = W;
        gphFbUploadTexInfo.h = hh;
        gphFbUploadTexInfo.tw = tw;
        gphFbUploadTexInfo.th = th;
        gphFbUploadTexInfo.data[0] = (void *)addr;
        GsTexLoadPacket(buf, &gphFbUploadTexInfo, 0xD3000, 0, 0, 1, 1, 0, 0, 0, 0, 0);
        FlushCache(0);
        GsTexUpload(buf);
        GsTexPacketSend(buf);
        fy2 = fy + fh;
        gphFbPacket->prim = 6;
        gphFbPacket->rgbaq = 0x3F80000080808080;
        ((u64 *)gphFbPacket)[0x380 / 8] = 0x80008; /* UV (u64), not st0 */
        gphFbPacket->xyz0 = fx | (fy << 16);
        ((u64 *)gphFbPacket)[0x390 / 8] = ((u64)((fh - 8) & 0xFFF8) << 16) | (((w << 4) - 8) & 0xFFF8);
        gphFbPacket->xyz1 = (u32)((fx + (w << 4) - 8) | ((fy2 - 8) << 16));
        __asm__ volatile("sync");
        fy = fy2;
        FlushCache(0);
        GsGifPacketSend(&gphFbPacket->dmaTag);
        GsFromSprDmaWait();
        addr += W * hh * 4;
    } while (done == 0);
}
