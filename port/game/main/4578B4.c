/* Port-only C for GphMeshDrawClip (0x004578B4), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "grx_dma.h"

/* The mesh pipeline's hand-written stages pass the mesh and their state in
   GPRs ($a0 mesh, $a2, $a3, $t0, $t9 = 0x70000000 the scratchpad base...)
   and in scratchpad words, so these drivers call them with no C arguments
   and set the registers the asm sets in the port's register file
   (grx_gpr[], grx_ee.h), leaving the rest as the stages left them */
enum { R_V0 = 2, R_A0 = 4, R_A1 = 5, R_A2 = 6, R_A3 = 7, R_T0 = 8, R_T1 = 9, R_T2 = 10, R_T7 = 15,
       R_S0 = 16, R_S1 = 17, R_T9 = 25 };

#define SPR_U8(off) (*(u8 *)(0x70000000 + (off)))
#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))

#define D8_CHCR (*(volatile u32 *)0x1000D000) /* fromSPR */
#define D8_MADR (*(volatile u32 *)0x1000D010)
#define D8_QWC (*(volatile u32 *)0x1000D020)
#define D8_SADR (*(volatile u32 *)0x1000D080)
#define D1_TADR (*(volatile u32 *)0x10009030) /* VIF1 */
#define D_RBSR (*(volatile u32 *)0x1000E040)

static void SetGpr(s32 r, u32 v) {
    grx_gpr[r].d[0] = v;
}

/* The asm keeps $s0-$s7 (128 bits each) in the scratchpad at 0x70000240
   while it runs, and $a1 at 0x70000220 (the stages read it there); its
   $ra goes to 0x70000224, which the port can't reproduce */
static void SaveRegs(void *tex) {
    s32 i;

    for (i = 0; i < 8; i++) {
        __builtin_memcpy((void *)(0x70000240 + i * 16), &grx_gpr[R_S0 + i], 16);
    }
    SPR_U32(0x220) = (u32)tex;
}

static void RestoreRegs(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        __builtin_memcpy(&grx_gpr[R_S0 + i], (void *)(0x70000240 + i * 16), 16);
    }
}

/* Sends the packet built in the current bounce buffer (0x700000F0, its
   end at 0x70000104) to the MFIFO ring with fromSPR, as GsMfifoSend does,
   then swaps the bounce buffers (0x70000A00 / 0x70001800). The DMA
   registers' values the asm leaves in $t0-$t5, $t7 and $s1-$s7 aren't
   put in grx_gpr. */
static void MeshKick(void) {
    u32 start = SPR_U32(0xF0);
    u32 qwc = (u32)(s32)(SPR_U32(0x104) - start) >> 4;
    u32 madr, tadr;

    while (D8_CHCR & 0x100) {
    }
    D8_SADR = start + 0x10000000;
    D8_QWC = qwc;
    for (;;) {
        madr = D8_MADR;
        tadr = D1_TADR;
        if (madr == tadr) {
            break;
        }
        if ((s32)(((tadr - madr) & D_RBSR) - (qwc << 4)) > 0) {
            break;
        }
    }
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, D8_CHCR | 0x100);
    SPR_U32(0xF0) = SPR_U32(0xF0) != 0x70000A00 ? 0x70000A00 : 0x70001800;
}

typedef void (*MeshStage)(void);

/* One unlit (mode & 0x1C == 0) or lit (== 8) pass: transform the vertices
   (billboarded when mode & 0x20) into the bounce buffer + work (0xD0
   unlit, 0xC0 lit; the address goes to 0x700000F4), cull the triangles
   into the transformed vertices' end + 0x10 or + 0x170 (0x700000F8 ->
   0x700000FC), and if any triangle is left (0x700000E0), number the
   vertices, build the packet and send it. Any other mode is the asm's
   break 0. */
static void MeshPass(u32 mode, MeshStage xform, MeshStage xformBillboard, MeshStage cull, MeshStage index,
                     MeshStage unlit, MeshStage lit) {
    u32 a2;
    u32 n;
    s32 isLit;

    SetGpr(R_T0, mode & 0x1C);
    a2 = SPR_U32(0xF0);
    SetGpr(R_A2, a2);
    if ((mode & 0x1C) == 0) {
        isLit = 0;
    } else {
        SetGpr(R_V0, 8);
        if ((mode & 0x1C) != 8) {
            __builtin_trap();
        }
        isLit = 1;
    }
    SetGpr(R_T0, isLit ? 0xC0 : 0xD0);
    a2 += isLit ? 0xC0 : 0xD0;
    SetGpr(R_A2, a2);
    SetGpr(R_T0, mode & 0x20);
    SPR_U32(0xF4) = a2;
    if (mode & 0x20) {
        xformBillboard();
    } else {
        xform();
    }
    a2 = SPR_U32(0xF8) + (isLit ? 0x170 : 0x10);
    SetGpr(R_A2, a2);
    SPR_U32(0xFC) = a2;
    cull();
    n = SPR_U32(0xE0);
    SetGpr(R_T0, n);
    if (n == 0) {
        return;
    }
    index();
    if (isLit) {
        lit();
    } else {
        unlit();
    }
    MeshKick();
}

extern u8 gphClipVerts[0x4000];
extern u8 gphClipTris[0x800];
extern u8 gphNearClipTriIdx[0x200];
extern u8 gphClipTriIdx[0x200];

void GphVertXformClip(void);
void GphVertXformClipBillboard(void);
void GphTriCullClip(void);
void GphTriCullClipNear(void);
void GphVertCompact(void);
void GphMeshPacketUnlit(void);
void GphMeshPacketLit(void);
void GphNearClipTris(void);
void GphClipTriGather(void);
void GphClipTriDraw(void);
void GphMeshDrawClip(GphGaObject *mesh, s32 tex);

/* Draws a mesh (the copy at 0x70003800) that crosses the screen edges:
   the triangles wholly inside go out as one packet (MeshPass), the rest
   are queued (write pointers at 0x70000020-0x7000002C, reset here to
   gphClipTris, gphClipVerts, gphClipTriIdx and gphNearClipTriIdx) and
   clipped on the EE afterwards. With clip bit 4 set (crossing the near
   plane) the cull also queues near-plane triangles, which are split first
   (GphNearClipTris). */
void GphMeshDrawClip(GphGaObject *mesh, s32 tex) {
    u32 mode;
    s32 nearClip;

    SetGpr(R_A0, (u32)mesh);
    SetGpr(R_A1, tex);
    SetGpr(R_T9, 0x70000000);
    SaveRegs((void *)tex);
    mode = mesh->mode;
    SPR_U8(0x94) = mode;
    SPR_U32(0x24) = (u32)gphClipVerts;
    SPR_U32(0x20) = (u32)gphClipTris;
    SPR_U32(0x2C) = (u32)gphNearClipTriIdx;
    SPR_U32(0x28) = (u32)gphClipTriIdx;
    SetGpr(R_T1, (u32)gphClipTris);
    SetGpr(R_T2, (u32)gphNearClipTriIdx);
    SetGpr(R_T0, mesh->clip);
    nearClip = mesh->clip & 4;
    SetGpr(R_T1, nearClip);
    mode = mesh->mode;
    SetGpr(R_A3, mode);
    MeshPass(mode, GphVertXformClip, GphVertXformClipBillboard, nearClip ? GphTriCullClipNear : GphTriCullClip,
             GphVertCompact, GphMeshPacketUnlit, GphMeshPacketLit);
    if (nearClip) {
        SetGpr(R_A2, SPR_U32(0x2C));
        SetGpr(R_A3, (u32)gphNearClipTriIdx);
        if (SPR_U32(0x2C) != (u32)gphNearClipTriIdx) {
            GphNearClipTris();
        }
    }
    SetGpr(R_T0, SPR_U32(0x28));
    SetGpr(R_T1, (u32)gphClipTriIdx);
    if (SPR_U32(0x28) != (u32)gphClipTriIdx) {
        GphClipTriGather();
    }
    GphClipTriDraw();
    RestoreRegs();
}
