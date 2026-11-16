/* Port-only C for GphPoly3DDraw (0x0043CE68), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "protos/main.h"

extern u8 gphPolyPrimTypes[3];
void GphPolyVertsProject(u32 n, u64 *out, FVECTOR *in);
void GphPoly3DDraw(u32 n, FVECTOR *verts, u32 color, s32 mode);

/* Queues a GIF packet drawing n projected vertices as one primitive of
   type gphPolyPrimTypes[mode & 3] (3 reads the byte after the table) in
   color. The packet is written through its uncached alias (| 0x20000000):
   DMA cnt tag, VIF DIRECT, a PRIM + RGBAQ (Q 1.0) REGLIST GIF tag, then an
   EOP XYZF2 REGLIST tag and the vertices, padded to a quadword. */
void GphPoly3DDraw(u32 n, FVECTOR *verts, u32 color, s32 mode) {
    u32 qw = (n + 1) >> 1;
    u32 pkt;
    u8 *u;
    u64 *v;

    pkt = GsPacketAlloc64((qw + 5) * 16);
    if (pkt == 0) {
        return;
    }
    u = (u8 *)(pkt | 0x20000000);
    *(u64 *)(u + 0x00) = (u32)((qw + 4) | 0x10000000);
    *(u64 *)(u + 0x08) = 0;
    *(u32 *)(u + 0x10) = 0;
    *(u32 *)(u + 0x14) = 0;
    *(u32 *)(u + 0x18) = 0;
    *(u32 *)(u + 0x1C) = ((qw + 3) & 0xFFFF) | 0x50000000;
    *(u64 *)(u + 0x20) = 0x2400000000000001ULL;
    *(u64 *)(u + 0x28) = 0x10;
    *(u64 *)(u + 0x30) = gphPolyPrimTypes[mode & 3];
    *(u64 *)(u + 0x38) = (u64)color | 0x3F80000000000000ULL;
    *(u64 *)(u + 0x40) = (u64)(n | 0x8000) | 0x1400000000000000ULL;
    *(u64 *)(u + 0x48) = 4;
    v = (u64 *)(u + 0x50);
    GphPolyVertsProject(n, v, verts);
    if (n & 1) {
        v[n] = 0;
    }
    GphPrimListAdd((s32)u);
}
