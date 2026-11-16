#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

extern s32 MemMove128(void *, void *, s32);

void GphBatchDrawLines(GphBatchLine *e) {
    GphBatchScratch *spr;
    GphBatchLinePkt *q;
    s32 buf;
    s32 last;
    s64 ox;
    s64 oy;
    u64 rq;
    u64 z;

    if (e->type != 0) {
        MemMove128((void *)0x70003180, &gphBatchPackets, 0xE8);
        spr = (GphBatchScratch *)0x70003180;
        buf = 0;
        last = -1;
        ox = gphBatchOffsetX + 0x800;
        spr->pkts.line[0].v[0].rgbaq = 0;
        oy = gphBatchOffsetY + 0x800;
        spr->pkts.line[1].v[0].rgbaq = 0;
        spr->pkts.line[0].v[0].xyz = 0;
        spr->pkts.line[1].v[0].xyz = 0;
        spr->pkts.line[0].v[1].rgbaq = 0;
        spr->pkts.line[1].v[1].rgbaq = 0;
        spr->pkts.line[0].v[1].xyz = 0;
        spr->pkts.line[1].v[1].xyz = 0;
        spr->pkts.line[0].end = 0;
        spr->pkts.line[1].end = 0;
        for (;;) {
            if (last != e->type) {
                GsRenderStatePacketSend(&spr->states[e->type - 1]);
                last = e->type;
            }
            rq = (u64)0x3F800000 << 32;
            q = &spr->pkts.line[buf];
            q->v[0].rgbaq = (u32)((e->c0[3] << 24) | (e->c0[0] | ((e->c0[1] << 8) | (e->c0[2] << 16)))) | rq;
            z = (u64)(e->z << 4) << 32;
            q->v[0].xyz = ((ox + e->x0) << 4) | ((oy + e->y0) << 20) | z;
            q->v[1].rgbaq = (u32)((e->c1[3] << 24) | (e->c1[0] | ((e->c1[1] << 8) | (e->c1[2] << 16)))) | rq;
            q->v[1].xyz = ((ox + e->x1) << 4) | ((oy + e->y1) << 20) | z;
            e++;
            GsFromSprDmaWait();
            GsGifPacketSend(q);
            if (e->type == 0) {
                break;
            }
            buf ^= 1;
        }
        GsFromSprDmaWait();
    }
}
