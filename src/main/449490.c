#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

extern void MemMove128(void *, void *, s32);
void GphBatchDrawQuadsColor(GphBatchQuad *s, u32 color)
{
  GphBatchScratch *spr;
  GphBatchQuadPkt *pkt;
  u64 ox;
  u64 oy;
  u64 rgbaq;
  u64 z;
  s32 buf;
  s32 last;
  GphBatchVtx *new_var;
  u32 t;
  if (s->type == 0)
  {
    return;
  }
  MemMove128((void *) 0x70003180, &gphBatchPackets, 0xE8);
  spr = (GphBatchScratch *) 0x70003180;
  ox = gphBatchOffsetX + 0x800;
  oy = gphBatchOffsetY + 0x800;
  rgbaq = color | (((u64) 0x3F800000) << 32);
  buf = 0;
  spr->pkts.line[1].v[0].rgbaq = 0;
  spr->pkts.sprite[0].rgbaq = 0;
 do { last = -1; } while (0); /* do-while(0): loop weight of last (allocation) */
  for (t = s->type;; buf ^= 1)
  {
    /* goto skip, not a while loop: an inner loop changes the allocation and layout */
    skip:
    if (t >> 3)
    {
      s++;
      t = s->type;
      goto skip;
    }

    if (last != t)
    {
      GsRenderStatePacketSend(&spr->states[t - 1]);
      last = s->type;
    }
    pkt = &spr->pkts.quad[buf];
    z = ((u64) ((u32) (s->z << 4))) << 32;
    pkt->v[0].rgbaq = rgbaq;
    pkt->v[0].uv = (((u64) ((s->v[0].v << 4) & 0xFFF0)) << 16) | ((s->v[0].u << 4) & 0xFFF0);
    pkt->v[0].xyz = (((ox + s->v[0].x) << 4) | ((oy + s->v[0].y) << 20)) | z;
    pkt->v[1].rgbaq = rgbaq;
    pkt->v[1].uv = (((u64) ((s->v[1].v << 4) & 0xFFF0)) << 16) | ((s->v[1].u << 4) & 0xFFF0);
    pkt->v[1].xyz = (((ox + s->v[1].x) << 4) | ((oy + s->v[1].y) << 20)) | z;
    new_var = &s->v[2];
    pkt->v[2].rgbaq = rgbaq;
    pkt->v[2].uv = (((u64) (((*new_var).v << 4) & 0xFFF0)) << 16) | ((s->v[2].u << 4) & 0xFFF0);
    pkt->v[2].xyz = (((ox + s->v[2].x) << 4) | ((oy + s->v[2].y) << 20)) | z;
    pkt->v[3].rgbaq = rgbaq;
    pkt->v[3].uv = (((u64) ((s->v[3].v << 4) & 0xFFF0)) << 16) | ((s->v[3].u << 4) & 0xFFF0);
    pkt->v[3].xyz = (((ox + s->v[3].x) << 4) | ((oy + s->v[3].y) << 20)) | z;
    s++;
    GsFromSprDmaWait();
    GsGifPacketSend(pkt);
    if (s->type == 0)
    {
      break;
    }
    t = s->type;
  }

  GsFromSprDmaWait();
}
