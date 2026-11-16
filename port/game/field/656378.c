/* Port-only C for WmapWarpSendGrid (0x00656378), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"

/* A grid vertex (WmapWarpCalcGrid 0x00656530) */
typedef struct WmapWarpVtx {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 pad;
    /* 0x10 */ f32 u;
    /* 0x14 */ f32 v;
    /* 0x18 */ u32 col;
    /* 0x1C */ u32 pad1C;
} WmapWarpVtx; /* size 0x20 */

#define WMAP_WARP_GRID 33

void WmapWarpSendGrid(void);

/* vftoi4: 4 fraction bits, truncated, saturating */
static s32 ftoi4(f32 x) {
    f32 f = x * 16.0f;

    if (f >= 2147483648.0f) {
        return 0x7FFFFFFF;
    }
    if (f <= -2147483648.0f) {
        return (s32)0x80000000;
    }
    return (s32)f;
}

/* One vertex in the GIF REGLIST order ST, RGBAQ, XYZ2 (24 bytes); x and y
   get the 2048 GS offset */
static u32 *putVtx(u32 *q, WmapWarpVtx *p) {
    f32 one = 1.0f;
    s32 x = ftoi4(p->x + 2048.0f);
    s32 y = ftoi4(p->y + 2048.0f);
    s32 z = ftoi4(p->z);

    __builtin_memcpy(&q[0], &p->u, 4);
    __builtin_memcpy(&q[1], &p->v, 4);
    q[2] = p->col;
    __builtin_memcpy(&q[3], &one, 4);
    q[4] = (x & 0xFFFF) | (y << 16);
    q[5] = z;
    return q + 6;
}

/* Draws the warp grid: per row of cells, one DMA cnt packet with a VIF
   DIRECT of a 66-vertex strip (this row's and the next row's vertices
   interleaved), sent from the scratchpad and waited for */
void WmapWarpSendGrid(void) {
    WmapWarpVtx *grid = (WmapWarpVtx *)WmapWarpGetGrid();
    u8 *pkt = (u8 *)WmapWarpGetPacket();
    WmapWarpVtx *a;
    WmapWarpVtx *b;
    u32 *q;
    s32 i;
    s32 j;

    for (i = 0; i < WMAP_WARP_GRID - 1; i++) {
        a = &grid[i * WMAP_WARP_GRID];
        b = &grid[(i + 1) * WMAP_WARP_GRID];
        *(u64 *)(pkt + 0x00) = 0x10000067;          /* DMA cnt, 0x67 qwords */
        *(u64 *)(pkt + 0x08) = 0;
        *(u32 *)(pkt + 0x10) = 0;
        *(u32 *)(pkt + 0x14) = 0;
        *(u32 *)(pkt + 0x18) = 0;
        *(u32 *)(pkt + 0x1C) = 0x50000066;          /* DIRECT 0x66 qwords */
        *(u64 *)(pkt + 0x20) = 0x1400000000008001ULL; /* REGLIST PRIM, 1 loop, EOP */
        *(u64 *)(pkt + 0x28) = 0;
        *(u64 *)(pkt + 0x30) = 4;                   /* PRIM: triangle strip */
        *(u64 *)(pkt + 0x38) = 0;
        *(u64 *)(pkt + 0x40) = 0x3400000000008042ULL; /* REGLIST x3, 66 loops, EOP */
        *(u64 *)(pkt + 0x48) = 0x512;               /* ST, RGBAQ, XYZ2 */
        q = (u32 *)(pkt + 0x50);
        for (j = 0; j < WMAP_WARP_GRID; j++) {
            q = putVtx(q, &a[j]);
            q = putVtx(q, &b[j]);
        }
        GsGifPacketSend((u16 *)pkt);
        GsFromSprDmaWait();
        pkt = (u8 *)q;
    }
}
