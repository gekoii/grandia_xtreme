#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0x140];
} Packet;

void GsTexUpload(Packet *);
void FlushCache(s32);

void GphFbUploadImage(s32 src) {
    Packet pkt;
    s32 last;
    u32 tw;
    u32 th;
    s32 i;
    u32 width;
    u32 rows;
    u32 h;
    u32 dst;
    u32 m;
    GphTex *d;

    th = 0xFF;
    width = 0x280;
    rows = 0x1C0;
    last = 0;
    tw = 0xFF;
    for (i = 0, m = 1; i < 11; i++, m <<= 1) {
        if (m >= width) {
            tw = i;
            break;
        }
    }
    dst = gsZBufAddr;
    do {
        if (width * rows > 0x14000) {
            h = 0x14000 / width;
            rows -= h;
        } else {
            h = rows;
            last = 1;
        }
        for (i = 0, m = 1; i < 11; i++, m <<= 1) {
            if (m >= h) {
                th = i;
                break;
            }
        }
        gphFbUploadTexInfo.data[0] = (void *)src;
        gphFbUploadTexInfo.w = width;
        gphFbUploadTexInfo.h = h;
        gphFbUploadTexInfo.tw = tw;
        gphFbUploadTexInfo.th = th;
        GsTexLoadPacket(&pkt, &gphFbUploadTexInfo, dst, 0, 0, 1, 1, 0, 0, 0, 0, 0);
        dst += 0x14000;
        FlushCache(0);
        GsTexUpload(&pkt);
        src += width * h * 4;
    } while (last == 0);
    /* one-pass loop: keeps %hi(gphFbTexInfo) a reload remat before the call */
    do {
        d = &gphFbTexInfo;
        FlushCache(0);
        GsTexLoadPacket(&gphFbPacket->texLoad.pkt, d, gsZBufAddr, 0, 0, 1, 1, 0, 0, 0, 0, 3);
        gphFbPacket->texLoad.reg.tex0 = 0
            + (u64)((u32)gsZBufAddr >> 6)
            + ((u64)10 << 14)
            + ((u64)d->psm << 20)
            + ((u64)d->tw << 26)
            + ((u64)d->th << 30)
            ;
        FlushCache(0);
        GsTexPacketSend(&gphFbPacket->texLoad.pkt);
    } while (0);
}
