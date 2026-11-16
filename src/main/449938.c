#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

extern s32 MemMove128(void *, void *, s32);

void GphBatchDrawSprites(GphBatchQuad *p) {
    GphBatchScratch *spr;
    GphBatchSpritePkt *pkt;
    s32 buf;
    s32 cur;
    s64 xo;
    s64 yo;
    u32 v;

    if (p->type == 0) {
        return;
    }
    MemMove128((void *)0x70003180, &gphBatchPackets, 0xE8);
    spr = (GphBatchScratch *)0x70003180;
    buf = 0;
    cur = -1;
    xo = gphBatchOffsetX + 0x800;
    yo = gphBatchOffsetY + 0x800;
    for (;;) {
    /* goto skip, not a while loop: an inner loop changes the allocation and layout */
    skip:
        v = p->type;
        if (v >> 3) {
            p++;
            goto skip;
        }
        if (cur != v) {
            GsRenderStatePacketSend(&spr->states[v - 1]);
            cur = p->type;
        }
        pkt = &spr->pkts.sprite[buf];
        pkt->rgbaq = (u64)(u32)((p->v[0].a << 24) | (p->v[0].b << 16) | (p->v[0].g << 8) | p->v[0].r) | ((u64)0x3F800000 << 32);
        pkt->v[0].uv = ((u64)((p->v[0].v << 4) & 0xFFF0) << 16) | ((p->v[0].u << 4) & 0xFFF0);
        pkt->v[0].xyz = ((xo + p->v[0].x) << 4) | ((yo + p->v[0].y) << 20) | ((u64)(p->z << 4) << 32);
        pkt->v[1].uv = ((u64)((p->v[1].v << 4) & 0xFFF0) << 16) | ((p->v[1].u << 4) & 0xFFF0);
        pkt->v[1].xyz = ((xo + p->v[1].x) << 4) | ((yo + p->v[1].y) << 20) | ((u64)(p->z << 4) << 32);
        p++;
        GsFromSprDmaWait();
        GsGifPacketSend(pkt);
        if (p->type == 0) {
            break;
        }
        buf ^= 1;
    }
    GsFromSprDmaWait();
}
