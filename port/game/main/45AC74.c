/* Port-only C for GphClipTriDraw (0x0045AC74), with its edge clippers
   gphClipOneYMin ... gphClipTwoXMax (0x0045B364-0x0045B960), which the
   decompilation still has in assembly (asm/main/main/058D48.s). */
/*
 * Register contract (GphMeshDrawClip, port/game/main/4578B4.c): $a0 = the
 * mesh, $t9 = 0x70000000; its input is the clip triangles in gphClipTris
 * up to scratchpad 0x70000020 (from GphNearClipTris/GphClipTriGather).
 * Nothing to do when there are none. Otherwise GphClipVertProject
 * (45B964.c) projects them into gphClipScreenVerts/gphClipScreenTris, and
 * each screen triangle is then:
 *   - dropped when all three outcodes (+0x3C, bits 0-3: past x max,
 *     x min, y max, y min of the clip bound at scratchpad 0x230) share a
 *     bit,
 *   - drawn when none is set: 3 x (STQ, RGBA, XYZF2) quadwords, the colour
 *     and position converted to integers (ftoi0),
 *   - else clipped at one edge (y min first, then y max, x min, x max):
 *     with one vertex outside, the two triangles that replace it, with two
 *     outside the one, are appended to the screen triangle list, so the
 *     loop meets them again for the other edges. The new vertices
 *     interpolate position, STQ (xyz), colour and clip x/y, and get the
 *     outcode of their clip point for the edges still to come.
 * The drawn triangles go out as one GS packet, PATH2 through VIF1 DIRECT,
 * built in gphClipPacketBuf at the address in scratchpad 0x70000014 (the
 * GIF tag from the header at 0x90, NLOOP = the count, EOP), sent with a
 * fromSPR chain at scratchpad 0x9E0 (a REF to the texture's GS packet
 * first when 0x70000220 is set) into the MFIFO ring. 0x70000014 advances
 * past the packet. That send (func_0045B64C in the asm: wait for fromSPR,
 * build the tags, wait for ring room, start it) is GsMfifoSend of the
 * tags at 0x700009E0, which the port calls instead of touching the DMA
 * registers; the asm's extra wait for fromSPR before it writes the tags
 * is left out (the port's DMA is synchronous, so the channel is idle).
 *
 * The edge clippers are VU0 macro code, done here as float C on the
 * port's register file grx_vf[] (the new vertices' STQ.w and clip z come
 * from lanes the asm doesn't write).
 *
 * Port differences: the asm restarts the packet buffer only once it has
 * passed 0x66000 bytes, but gphClipPacketBuf is 0x58400 bytes, so on the
 * PS2 a busy frame writes on into gphClipVerts and beyond. Here the buffer
 * restarts when a packet of the largest possible size (every slot of
 * gphClipScreenTris drawn) might not fit; with the port's synchronous DMA
 * the old packets are already sent. The asm also has a second copy of the
 * draw loop at 0x0045ADA0 that also copies 3 quadwords of the material per
 * triangle and uses the GIF tag at 0xA0; nothing reaches it, so it isn't
 * ported.
 */
#include "common.h"
#include "gph.h"
#include "grx_memory.h"
#include "vu0_port.h"

enum { R_A0 = 4 };

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))
#define VF(n) (grx_vf[n].f)

extern u8 gphClipTris[0x800];
extern u8 gphClipScreenTris[0x4000];
extern u8 gphClipPacketBuf[0x58400];

void GphClipVertProject(void);
void GsMfifoSend(void *pkt, s32 qwc);
void GphClipTriDraw(void);

/* The largest packet: the DIRECT and GIF tag quadwords and 9 quadwords
   for every gphClipScreenTris slot */
#define CLIP_PACKET_MAX (0x20 + (sizeof(gphClipScreenTris) / 16) * 0x90)

/* The clipping state the asm keeps in registers */
typedef struct {
    u32 listEnd; /* $a1: the screen triangle list's end, grows */
    u32 entry;   /* $s5: the triangle being clipped */
} ClipState;

/* clipw.xyz p, w: bit 0 x > +|w|, 1 x < -|w|, 2/3 y, 4/5 z */
static u32 ClipFlags(const f32 *p, f32 w) {
    u32 c = 0;

    if (w < 0.0f) {
        w = -w;
    }
    if (p[0] > w) {
        c |= 1;
    }
    if (p[0] < -w) {
        c |= 2;
    }
    if (p[1] > w) {
        c |= 4;
    }
    if (p[1] < -w) {
        c |= 8;
    }
    if (p[2] > w) {
        c |= 0x10;
    }
    if (p[2] < -w) {
        c |= 0x20;
    }
    return c;
}

/* The 4 quadwords of screen vertex v into $vf(5+j), $vf(10+j), $vf(15+j),
   $vf(20+j) */
static void LoadVert(u32 v, s32 j) {
    __builtin_memcpy(VF(5 + j), (void *)v, 16);
    __builtin_memcpy(VF(10 + j), (void *)(v + 0x10), 16);
    __builtin_memcpy(VF(15 + j), (void *)(v + 0x20), 16);
    __builtin_memcpy(VF(20 + j), (void *)(v + 0x30), 16);
}

static void PutTri(u32 dst, u32 a, u32 b, u32 c, u32 mat) {
    u32 *t = (u32 *)dst;

    t[0] = a;
    t[1] = b;
    t[2] = c;
    t[3] = mat;
}

/* One vertex (c) outside: slots (a, b, c); the new vertices n1 (on c-a)
   and n2 (on c-b) at 0x7000001C, the triangles (a, b, n1), (b, n2, n1) */
static void SetupOne(ClipState *st, u32 a, u32 b, u32 c) {
    u32 s1 = SPR_U32(0x1C);
    u32 mat = ((u32 *)st->entry)[3];

    LoadVert(a, 0);
    LoadVert(b, 1);
    LoadVert(c, 2);
    PutTri(st->listEnd, a, b, s1, mat);
    PutTri(st->listEnd + 0x10, b, s1 + 0x40, s1, mat);
    st->listEnd += 0x20;
}

/* Two vertices (b, c) outside: slots (a, b, c); the new vertices on a-b
   and a-c, the triangle (a, n1, n2) */
static void SetupTwo(ClipState *st, u32 a, u32 b, u32 c) {
    u32 s1 = SPR_U32(0x1C);
    u32 mat = ((u32 *)st->entry)[3];

    LoadVert(a, 0);
    LoadVert(b, 1);
    LoadVert(c, 2);
    PutTri(st->listEnd, a, s1, s1 + 0x40, mat);
    st->listEnd += 0x10;
}

/* The two new vertices: d1 = base - diff1 * t1 and d2 = base - diff2 * t2
   for every attribute ($vf4.x/y = t1/t2); from the outside vertex c
   (slot 2) for "one", from the inside vertex a (slot 0) for "two". Their
   clip points get outcodes & keep (the edges still to clip). */
static void ClipNew(s32 base, s32 o1, s32 o2, u32 keep) {
    f32 t1 = VF(4)[0], t2 = VF(4)[1];
    u32 s1 = SPR_U32(0x1C);
    f32 *v2 = VF(2), *v3 = VF(3);
    s32 i;

    /* clip x/y, from the setup's $vf2/$vf3 */
    for (i = 0; i < 2; i++) {
        VF(23)[i] = VF(20 + base)[i] - v2[i] * t1;
        VF(24)[i] = VF(20 + base)[i] - v3[i] * t2;
    }
    for (i = 0; i < 4; i++) {
        v2[i] = VF(5 + base)[i] - VF(5 + o1)[i];
        v3[i] = VF(5 + base)[i] - VF(5 + o2)[i];
        VF(8)[i] = VF(5 + base)[i] - v2[i] * t1;
        VF(9)[i] = VF(5 + base)[i] - v3[i] * t2;
    }
    for (i = 0; i < 3; i++) {
        v2[i] = VF(10 + base)[i] - VF(10 + o1)[i];
        v3[i] = VF(10 + base)[i] - VF(10 + o2)[i];
        VF(13)[i] = VF(10 + base)[i] - v2[i] * t1;
        VF(14)[i] = VF(10 + base)[i] - v3[i] * t2;
    }
    for (i = 0; i < 4; i++) {
        v2[i] = VF(15 + base)[i] - VF(15 + o1)[i];
        v3[i] = VF(15 + base)[i] - VF(15 + o2)[i];
        VF(18)[i] = VF(15 + base)[i] - v2[i] * t1;
        VF(19)[i] = VF(15 + base)[i] - v3[i] * t2;
    }
    __builtin_memcpy((void *)s1, VF(8), 16);
    __builtin_memcpy((void *)(s1 + 0x10), VF(13), 16);
    __builtin_memcpy((void *)(s1 + 0x20), VF(18), 16);
    __builtin_memcpy((void *)(s1 + 0x30), VF(23), 16);
    *(u32 *)(s1 + 0x3C) = ClipFlags(VF(23), VF(25)[3]) & keep;
    __builtin_memcpy((void *)(s1 + 0x40), VF(9), 16);
    __builtin_memcpy((void *)(s1 + 0x50), VF(14), 16);
    __builtin_memcpy((void *)(s1 + 0x60), VF(19), 16);
    __builtin_memcpy((void *)(s1 + 0x70), VF(24), 16);
    *(u32 *)(s1 + 0x7C) = ClipFlags(VF(24), VF(25)[3]) & keep;
    SPR_U32(0x1C) = s1 + 0x80;
}

/* The edges: bit 8 y min, 4 y max, 2 x min, 1 x max. lane: the clip point
   lane (y or x), bound: the lane of scratchpad 0x230 holding the edge
   (x = min, y = max), keep: the outcode bits still to clip */
typedef struct {
    u32 bit;
    s32 lane;
    s32 bound;
    u32 keep;
} ClipEdge;

static const ClipEdge sClipEdges[4] = {
    { 8, 1, 0, 7 },
    { 4, 1, 1, 3 },
    { 2, 0, 0, 1 },
    { 1, 0, 1, 0 },
};

/* gphClipOne*: the slot-2 vertex is outside the edge */
static void ClipOne(const ClipEdge *e) {
    f32 *v1 = VF(1);

    __builtin_memcpy(VF(25), (void *)0x70000230, 16);
    v1[e->lane] = VF(22)[e->lane] - VF(25)[e->bound];
    VF(2)[0] = VF(22)[0] - VF(20)[0];
    VF(2)[1] = VF(22)[1] - VF(20)[1];
    VF(3)[0] = VF(22)[0] - VF(21)[0];
    VF(3)[1] = VF(22)[1] - VF(21)[1];
    VF(4)[0] = VuDiv(v1[e->lane], VF(2)[e->lane]);
    VF(4)[1] = VuDiv(v1[e->lane], VF(3)[e->lane]);
    ClipNew(2, 0, 1, e->keep);
}

/* gphClipTwo*: the slot-0 vertex is the one inside */
static void ClipTwo(const ClipEdge *e) {
    f32 *v1 = VF(1);

    __builtin_memcpy(VF(25), (void *)0x70000230, 16);
    v1[e->lane] = VF(20)[e->lane] - VF(25)[e->bound];
    VF(2)[0] = VF(20)[0] - VF(21)[0];
    VF(2)[1] = VF(20)[1] - VF(21)[1];
    VF(3)[0] = VF(20)[0] - VF(22)[0];
    VF(3)[1] = VF(20)[1] - VF(22)[1];
    VF(4)[0] = VuDiv(v1[e->lane], VF(2)[e->lane]);
    VF(4)[1] = VuDiv(v1[e->lane], VF(3)[e->lane]);
    ClipNew(0, 1, 2, e->keep);
}

/* Clips triangle (v0, v1, v2) with outcodes c0-c2 (no bit shared, some
   set) at the first edge any of them is past */
static void ClipTri(ClipState *st, u32 v0, u32 v1, u32 v2, u32 c0, u32 c1, u32 c2) {
    u32 any = c0 | c1 | c2;
    const ClipEdge *e;
    u32 b;

    for (e = sClipEdges; !(any & e->bit) && e < &sClipEdges[3]; e++) {
    }
    b = e->bit;
    if (!(c0 & b)) {
        if (!(c1 & b)) {
            SetupOne(st, v0, v1, v2);
            ClipOne(e);
        } else if (c2 & b) {
            SetupTwo(st, v0, v1, v2);
            ClipTwo(e);
        } else {
            SetupOne(st, v2, v0, v1);
            ClipOne(e);
        }
    } else if (c1 & b) {
        SetupTwo(st, v2, v0, v1);
        ClipTwo(e);
    } else if (c2 & b) {
        SetupTwo(st, v1, v2, v0);
        ClipTwo(e);
    } else {
        SetupOne(st, v1, v2, v0);
        ClipOne(e);
    }
}

/* One vertex of a drawn triangle: STQ, RGBA (ftoi0), XYZF2 (ftoi0) */
static void PutGsVert(u32 dst, u32 v) {
    s32 *rgba = (s32 *)(dst + 0x10);
    s32 *xyzf = (s32 *)(dst + 0x20);
    const f32 *c = (const f32 *)(v + 0x20);
    const f32 *p = (const f32 *)v;
    s32 i;

    __builtin_memcpy((void *)dst, (void *)(v + 0x10), 16);
    for (i = 0; i < 4; i++) {
        rgba[i] = VuFtoi(c[i], 0);
        xyzf[i] = VuFtoi(p[i], 0);
    }
}

/* Clips gphClipScreenTris against the four screen edges and sends the GS
   packet (see the top) */
void GphClipTriDraw(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 base = (u32)gphClipPacketBuf + GRX_UNCACHED;
    ClipState st;
    u32 v1, s3, count, qw;

    if (SPR_U32(0x20) == (u32)gphClipTris) {
        return;
    }
    grx_gpr[5].d[0] = mesh->mode;
    GphClipVertProject();
    st.listEnd = SPR_U32(0x18);
    v1 = SPR_U32(0x14);
    if ((s32)(v1 - base) < 0 || v1 - base + CLIP_PACKET_MAX > sizeof(gphClipPacketBuf)) {
        v1 = base;
    }
    s3 = v1 + 0x20;
    count = 0;
    for (st.entry = (u32)gphClipScreenTris; st.entry != st.listEnd; st.entry += 0x10) {
        const u32 *t = (const u32 *)st.entry;
        u32 c0 = *(u32 *)(t[0] + 0x3C);
        u32 c1 = *(u32 *)(t[1] + 0x3C);
        u32 c2 = *(u32 *)(t[2] + 0x3C);

        if (c0 & c1 & c2 & 0xF) {
            continue;
        }
        if (c0 | c1 | c2) {
            ClipTri(&st, t[0], t[1], t[2], c0, c1, c2);
            continue;
        }
        PutGsVert(s3, t[0]);
        PutGsVert(s3 + 0x30, t[1]);
        PutGsVert(s3 + 0x60, t[2]);
        s3 += 0x90;
        count++;
    }
    SPR_U32(0x14) = s3;
    if (count == 0) {
        return;
    }
    qw = (s3 - v1 - 0x20) >> 4;
    __builtin_memset((void *)v1, 0, 16);
    *(u32 *)(v1 + 0xC) = 0x50000001 + qw;
    __builtin_memcpy((void *)(v1 + 0x10), (void *)0x70000090, 16);
    *(u16 *)(v1 + 0x10) = (u16)(count | 0x8000);
    /* Port: bit 44 of the GIF tag (unused by the GS) marks the packet as
       3D for widescreen (port/src/gs/gs_widescreen.c) */
    *(u32 *)(v1 + 0x14) |= 1u << 12;
    {
        u32 t3 = 0x700009E0;
        u32 tex = SPR_U32(0x220);
        u32 tags;

        if (tex != 0) {
            u32 addr = tex + 0x10;

            if ((s32)(tex - 0x70000000) >= 0) {
                addr += 0x10000000;
            }
            *(u32 *)t3 = 0x30000000 + *(u16 *)tex;
            *(u32 *)(t3 + 4) = addr;
            *(u32 *)(t3 + 0x10) = 0x30000002 + qw;
            *(u32 *)(t3 + 0x14) = v1 & 0x0FFFFFFF;
            tags = 2;
        } else {
            *(u32 *)t3 = 0x30000002 + qw;
            *(u32 *)(t3 + 4) = v1 & 0x0FFFFFFF;
            tags = 1;
        }
        GsMfifoSend((void *)t3, tags);
    }
}
