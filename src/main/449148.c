#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

extern s32 MemMove128(void *, void *, s32);

void GphBatchDrawQuads(GphBatchQuad *p) {
    GphBatchScratch *spr;
    GphBatchQuadPkt *pkt;
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
    spr->pkts.line[1].v[0].rgbaq = 0;
    spr->pkts.sprite[0].rgbaq = 0;
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
        pkt = &spr->pkts.quad[buf];
        pkt->v[0].rgbaq = (u64)(u32)((p->v[0].a << 24) | (p->v[0].b << 16) | (p->v[0].g << 8) | p->v[0].r) | ((u64)0x3F800000 << 32);
        pkt->v[0].uv = ((u64)((p->v[0].v << 4) & 0xFFF0) << 16) | ((p->v[0].u << 4) & 0xFFF0);
        pkt->v[0].xyz = ((xo + p->v[0].x) << 4) | ((yo + p->v[0].y) << 20) | ((u64)(p->z << 4) << 32);
        pkt->v[1].rgbaq = (u64)(u32)((p->v[1].a << 24) | (p->v[1].b << 16) | (p->v[1].g << 8) | p->v[1].r) | ((u64)0x3F800000 << 32);
        pkt->v[1].uv = ((u64)((p->v[1].v << 4) & 0xFFF0) << 16) | ((p->v[1].u << 4) & 0xFFF0);
        pkt->v[1].xyz = ((xo + p->v[1].x) << 4) | ((yo + p->v[1].y) << 20) | ((u64)(p->z << 4) << 32);
        pkt->v[2].rgbaq = (u64)(u32)((p->v[2].a << 24) | (p->v[2].b << 16) | (p->v[2].g << 8) | p->v[2].r) | ((u64)0x3F800000 << 32);
        pkt->v[2].uv = ((u64)((p->v[2].v << 4) & 0xFFF0) << 16) | ((p->v[2].u << 4) & 0xFFF0);
        pkt->v[2].xyz = ((xo + p->v[2].x) << 4) | ((yo + p->v[2].y) << 20) | ((u64)(p->z << 4) << 32);
        pkt->v[3].rgbaq = (u64)(u32)((p->v[3].a << 24) | (p->v[3].b << 16) | (p->v[3].g << 8) | p->v[3].r) | ((u64)0x3F800000 << 32);
        pkt->v[3].uv = ((u64)((p->v[3].v << 4) & 0xFFF0) << 16) | ((p->v[3].u << 4) & 0xFFF0);
        pkt->v[3].xyz = ((xo + p->v[3].x) << 4) | ((yo + p->v[3].y) << 20) | ((u64)(p->z << 4) << 32);
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
