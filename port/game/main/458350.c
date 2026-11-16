/* Port-only C for the mesh pipeline's VU1 packet builders, which the
   decompilation still has in assembly (asm/main/main/0578B4.s):
   GphMeshPacketUnlit (0x00458350), GphMeshPacketUnlitVu1 (0x004584F0),
   GphMeshPacketLit (0x00458748) and GphMeshPacketLitVu1 (0x004589DC). */
/*
 * Register contract (the drivers, port/game/main/4578B4.c ...): $a0 = the
 * mesh, $t9 = 0x70000000. They read the scratchpad words the earlier
 * stages left: 0x700000F0 (the bounce buffer the packet starts in),
 * 0x700000F4/F8/FC/100 (the vertex packet, its end, the triangle packet,
 * its end; VU0 paths only), 0x700000E0/E4 (triangle and used-vertex
 * counts), 0x70000220 (the texture's GS packet, or 0), and the view's
 * header quadwords at 0x70000060-0x700000C0. Output: the packet and its
 * end in 0x70000104, which the driver sends (MeshKick). They also write
 * the per-mesh words into the header block (0x78, 0x80-0x8C, the GIF
 * tag's NLOOP at 0x90, 0xC0, and 0xCC for the lit ones). No caller reads
 * the registers the asm leaves.
 *
 * The packet is a DMA chain for VIF1 (port/docs/vu.md "VU1: the mesh
 * renderer"): a CNT tag with the header (10 quadwords for the VU0 paths,
 * 14 for the VU1 paths, UNPACKed to VU address 0x1A0), REF tags to the
 * mesh's arrays (with their UNPACK headers in the first quadword), the
 * VIF code templates in the scratchpad (REF 0x80000110/120/130/170: MSCAL
 * and MSCNT sequences), the light block in a CNT, and at the end the
 * texture's GS packet (REF, DIRECT) when there is one. Quadwords and words
 * the asm doesn't write are left as they are in the bounce buffer.
 */
#include "common.h"
#include "gph.h"

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))

enum { R_A0 = 4 };

void GphMeshPacketUnlit(void);
void GphMeshPacketUnlitVu1(void);
void GphMeshPacketLit(void);
void GphMeshPacketLitVu1(void);

static void W32(u32 addr, u32 v) {
    *(u32 *)addr = v;
}

/* sd: a 64-bit store of a 32-bit value (the high word 0) */
static void W64(u32 addr, u32 v) {
    *(u32 *)addr = v;
    *(u32 *)(addr + 4) = 0;
}

static void Zero16(u32 addr) {
    __builtin_memset((void *)addr, 0, 16);
}

static void Copy16(u32 dst, u32 src) {
    __builtin_memcpy((void *)dst, (const void *)src, 16);
}

/* The header words all four write first. vu1: the VU1 paths (first slot
   0, the vertex count from 0x700000E4) */
static void HeaderWords(GphGaObject *mesh, s32 vu1) {
    u32 tris = SPR_U32(0xE0);

    __builtin_memcpy((void *)0x70000078, &mesh->unk70, 8);
    if (vu1) {
        SPR_U32(0x80) = 0;
        SPR_U32(0x84) = SPR_U32(0xE4);
    } else {
        SPR_U32(0x80) = mesh->vtxCount - SPR_U32(0xE4);
        SPR_U32(0x84) = mesh->vtxCount;
    }
    SPR_U32(0x88) = tris;
    *(u16 *)0x70000090 = (u16)(tris | 0x8000);
    __builtin_memcpy((void *)0x700000C0, &mesh->uOffset, 8);
}

/* The packet's first tag: CNT qwc (the header's size + 1), NOP NOP NOP
   UNPACK V4-32 of size-1 quadwords to 0x1A0 */
static void HeaderTag(u32 a2, u32 qwc, u32 unpack) {
    W64(a2 + 8, 0);
    W64(a2, 0x10000000 | qwc);
    Zero16(a2 + 0x10);
    W32(a2 + 0x1C, unpack);
}

/* REF tag at a: qwc quadwords at addr */
static void Ref(u32 a, u32 qwc, u32 addr) {
    W32(a, 0x30000000 + qwc);
    W32(a + 4, addr);
}

/* The 17 quadwords of the light block (mesh +0x3C) after a CNT 17 tag */
static void LightBlock(GphGaObject *mesh, u32 a) {
    u32 lb = (u32)mesh->lightBlock;
    s32 i;

    W32(a, 0x10000011);
    for (i = 0; i < 17; i++) {
        Copy16(a + 0x10 + i * 16, lb + i * 16);
    }
}

/* The end, from 0x70000100: when there is a texture GS packet, CNT 1
   (FLUSH), then a REF to the packet's quadwords after its first (their
   count is its first halfword; the scratchpad bit added when it is in the
   scratchpad); then REF template 0x120. The end goes to 0x70000104. The
   REF tags' upper words are left as they are, as in the asm (VIF1's
   CHCR.TTE, which decides whether they reach the VIF, is kept from before
   GsMfifoStart). */
static void PacketEnd(u32 a3) {
    u32 tex = SPR_U32(0x220);

    if (tex != 0) {
        u32 qwc;
        u32 addr;

        Zero16(a3 + 0x10);
        W32(a3, 0x10000001);
        W32(a3 + 0x10, 0x11000000);
        qwc = *(u16 *)tex;
        addr = tex + 0x10;
        if ((s32)(tex - 0x70000000) >= 0) {
            addr += 0x10000000;
        }
        Ref(a3 + 0x20, qwc, addr);
        W32(a3 + 0x30, 0x30000001);
        W32(a3 + 0x34, 0x80000120);
        a3 += 0x40;
    } else {
        W32(a3, 0x30000001);
        W32(a3 + 4, 0x80000120);
        a3 += 0x10;
    }
    SPR_U32(0x104) = a3;
}

/* VIF packet for a VU0-transformed mesh: header, refs to the colours
   (+0x6C) and UVs (+0x68), MSCAL 2 (VU1 0x010) */
void GphMeshPacketUnlit(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = SPR_U32(0xF0);
    u32 n;
    u32 a3;

    HeaderWords(mesh, 0);
    HeaderTag(a2, 0xB, 0x6C0A01A0);
    Copy16(a2 + 0x20, 0x70000060);
    Copy16(a2 + 0x30, 0x70000070);
    Copy16(a2 + 0x40, 0x70000080);
    Copy16(a2 + 0x50, 0x70000090);
    Copy16(a2 + 0x60, (u32)mesh->ambient);
    Copy16(a2 + 0xA0, 0x700000B0);
    Copy16(a2 + 0xB0, 0x700000C0);
    n = mesh->vtxCount;
    Ref(a2 + 0xC0, ((n + 3) >> 2) + 1, (u32)mesh->colors);
    W32(SPR_U32(0xF4) + 0x10, 0x14000002);
    Ref(SPR_U32(0xF8), ((n + 3) >> 2) + 1, (u32)mesh->uvs);
    a3 = SPR_U32(0xFC);
    W32(a3 + 0x10, 0x10000000);
    W32(a3 + 0x14, 0x17000000);
    PacketEnd(SPR_U32(0x100));
}

/* The same for a mesh transformed on VU1: the matrix in the header, then
   colours, positions and UVs by REF, MSCAL 0x1A + 2 * (mode & 3), the
   triangle list (+0x50) after FLUSHE, MSCNT */
void GphMeshPacketUnlitVu1(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = SPR_U32(0xF0);
    u32 nv, nt;
    u32 mtx = (u32)mesh->mtx;
    u32 a3;

    HeaderWords(mesh, 1);
    HeaderTag(a2, 0xF, 0x6C0E01A0);
    Copy16(a2 + 0x20, 0x70000060);
    Copy16(a2 + 0x30, 0x70000070);
    Copy16(a2 + 0x40, 0x70000080);
    Copy16(a2 + 0x50, 0x70000090);
    Copy16(a2 + 0x60, (u32)mesh->ambient);
    Copy16(a2 + 0xA0, mtx);
    Copy16(a2 + 0xB0, mtx + 0x10);
    Copy16(a2 + 0xC0, mtx + 0x20);
    Copy16(a2 + 0xD0, mtx + 0x30);
    Copy16(a2 + 0xE0, 0x700000B0);
    Copy16(a2 + 0xF0, 0x700000C0);
    nv = SPR_U32(0xE4);
    nt = SPR_U32(0xE0);
    a3 = a2 + 0x100;
    Ref(a3, ((nv + 3) >> 2) + 1, (u32)mesh->colors);
    a3 += 0x10;
    W64(a3, 0x10000001);
    Zero16(a3 + 0x10);
    W32(a3 + 0x1C, 0x6C000000 | (nv << 16));
    Ref(a3 + 0x20, nv, (u32)mesh->verts);
    a3 += 0x30;
    W64(a3, 0x10000001);
    Zero16(a3 + 0x10);
    W32(a3 + 0x10, 0x1400001A + (mesh->mode & 3) * 2);
    Ref(a3 + 0x20, ((nv + 3) >> 2) + 1, (u32)mesh->uvs);
    a3 += 0x30;
    W64(a3, 0x10000001);
    W32(a3 + 0x10, 0x10000000);
    W32(a3 + 0x14, 0x17000000);
    W32(a3 + 0x18, 0);
    W32(a3 + 0x1C, 0x6D00017F | (((nt + 1) & 0xFFFE) << 16));
    Ref(a3 + 0x20, (nt + 1) >> 1, (u32)mesh->indices);
    PacketEnd(a3 + 0x30);
}

/* VU0-transformed mesh with normals (+0x64) and the 0x110-byte light block
   (+0x3C), VU1 0x030 through template 0x110 */
void GphMeshPacketLit(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = SPR_U32(0xF0);
    u32 mtx2 = (u32)mesh->mtx2;
    u32 n;
    u32 a3;

    __builtin_memcpy((void *)0x700000CC, &mesh->unk30, 4);
    HeaderWords(mesh, 0);
    HeaderTag(a2, 0xB, 0x6C0A01A0);
    Copy16(a2 + 0x20, 0x70000060);
    Copy16(a2 + 0x30, 0x70000070);
    Copy16(a2 + 0x40, 0x70000080);
    Copy16(a2 + 0x50, 0x70000090);
    Copy16(a2 + 0x60, (u32)mesh->ambient);
    Copy16(a2 + 0x70, mtx2);
    Copy16(a2 + 0x80, mtx2 + 0x10);
    Copy16(a2 + 0x90, mtx2 + 0x20);
    Copy16(a2 + 0xA0, 0x700000B0);
    Copy16(a2 + 0xB0, 0x700000C0);
    n = mesh->vtxCount;
    a3 = SPR_U32(0xF8);
    Ref(a3, ((n + 1) >> 1) + 1, (u32)mesh->normals);
    Ref(a3 + 0x10, 1, 0x80000110);
    Ref(a3 + 0x20, ((n + 3) >> 2) + 1, (u32)mesh->colors);
    LightBlock(mesh, a3 + 0x30);
    Ref(a3 + 0x150, 1, 0x80000130);
    Ref(a3 + 0x160, ((n + 3) >> 2) + 1, (u32)mesh->uvs);
    W32(SPR_U32(0xFC) + 0x10, 0x17000000);
    PacketEnd(SPR_U32(0x100));
}

/* Lit mesh transformed on VU1: MSCAL 0x22 + 2 * (mode & 3), normals and
   template 0x170 (MSCNT), colours, light block, template 0x130, UVs, the
   triangle list */
void GphMeshPacketLitVu1(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = SPR_U32(0xF0);
    u32 mtx = (u32)mesh->mtx;
    u32 mtx2 = (u32)mesh->mtx2;
    u32 nv, nt;
    u32 a3;

    __builtin_memcpy((void *)0x700000CC, &mesh->unk30, 4);
    HeaderWords(mesh, 1);
    HeaderTag(a2, 0xF, 0x6C0E01A0);
    Copy16(a2 + 0x20, 0x70000060);
    Copy16(a2 + 0x30, 0x70000070);
    Copy16(a2 + 0x40, 0x70000080);
    Copy16(a2 + 0x50, 0x70000090);
    Copy16(a2 + 0x60, (u32)mesh->ambient);
    Copy16(a2 + 0x70, mtx2);
    Copy16(a2 + 0x80, mtx2 + 0x10);
    Copy16(a2 + 0x90, mtx2 + 0x20);
    Copy16(a2 + 0xA0, mtx);
    Copy16(a2 + 0xB0, mtx + 0x10);
    Copy16(a2 + 0xC0, mtx + 0x20);
    Copy16(a2 + 0xD0, mtx + 0x30);
    Copy16(a2 + 0xE0, 0x700000B0);
    Copy16(a2 + 0xF0, 0x700000C0);
    nv = SPR_U32(0xE4);
    nt = SPR_U32(0xE0);
    a3 = a2 + 0x100;
    W64(a3, 0x10000001);
    Zero16(a3 + 0x10);
    W32(a3 + 0x1C, 0x6C000000 | (nv << 16));
    Ref(a3 + 0x20, nv, (u32)mesh->verts);
    a3 += 0x30;
    W64(a3, 0x10000001);
    Zero16(a3 + 0x10);
    W32(a3 + 0x10, 0x14000022 + (mesh->mode & 3) * 2);
    a3 += 0x20;
    Ref(a3, ((nv + 1) >> 1) + 1, (u32)mesh->normals);
    Ref(a3 + 0x10, 1, 0x80000170);
    Ref(a3 + 0x20, ((nv + 3) >> 2) + 1, (u32)mesh->colors);
    LightBlock(mesh, a3 + 0x30);
    Ref(a3 + 0x150, 1, 0x80000130);
    Ref(a3 + 0x160, ((nv + 3) >> 2) + 1, (u32)mesh->uvs);
    a3 += 0x170;
    W64(a3, 0x10000001);
    W32(a3 + 0x10, 0x10000000);
    W32(a3 + 0x14, 0x17000000);
    W32(a3 + 0x18, 0);
    W32(a3 + 0x1C, 0x6D00017F | (((nt + 1) & 0xFFFE) << 16));
    Ref(a3 + 0x20, (nt + 1) >> 1, (u32)mesh->indices);
    PacketEnd(a3 + 0x30);
}
