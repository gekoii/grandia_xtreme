/* main executable .data 0x0049A970-0x0049AD80: graphics packets and
 * tables, the heap ids, the frame-buffer texture descriptors, the ripple
 * buffers and the particle strip's light block. Data-only C (plan step 6b,
 * docs/shifting.md); the users are 43ADC8.c (projected texture), 43C630.c
 * and 43CE68 (primitives), 43E060 (Hermite curves), 440490.c (heaps), the
 * frame-buffer effects (gph.h), 447C20/448068 (ripple), 4475B8.c and
 * 44A1F8.c (PtclInit).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049A970-0x0049AD80 */

typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* GS A+D register packet (gph.h GsAdPkt, laid out by GsAdPktInit 43B9E0):
 * DMA cnt tag, VIF NOP x3 + DIRECT, GIF tag (A+D), then the registers. A
 * DMA source, so 16-aligned. */
typedef struct {
    u64 data;
    u64 addr;
} GsAdReg;

typedef struct {
    u64 dmaTag;                 /* cnt, qword count */
    u64 dmaTagHi;
    s32 vif[3];                 /* NOP */
    s32 direct;                 /* VIF DIRECT, qword count */
    u64 gifTag;
    u64 gifRegs;                /* 0xE = A+D */
    GsAdReg reg[5];
} __attribute__((aligned(16))) GsAdPkt; /* size 0x80 */

/* GS registers */
#define GS_CLAMP_1      0x08
#define GS_PRMODE       0x1B
#define GS_ALPHA_1      0x42
#define GS_TEST_1       0x47

/* Projected texture draw state (43ADC8): 4 registers */
GsAdPkt gphProjTexGsPkt = {
    0x10000006, 0,
    { 0, 0, 0 }, 0x50000005,
    0x1000400000008004ULL, 0xE,
    {
        { 0x78, GS_PRMODE },
        { 5, GS_CLAMP_1 },
        { 0x8000000044ULL, GS_ALPHA_1 },
        { 0x5040A, GS_TEST_1 },
    },
};

/* 43C630: set once a second (every 60 frames) */
s32 gphPrimSecondFlag = 0;
s32 gphPrimFrameCount = 0;

/* GS PRIM types by the low two bits of the polygon kind (43CE68):
 * triangle, triangle strip, triangle fan */
u8 gphPolyPrimTypes[3] = { 3, 4, 5 };

/* Hermite basis matrix (43E060, loaded with lqc2) */
FVECTOR vuHermiteBasis[4] = {
    { 2.0f, -2.0f, 1.0f, 1.0f },
    { -3.0f, 3.0f, -2.0f, -1.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
};

/* heaps (main/n06.h) */
s32 memCurHeapId = 0;           /* MemHeapSelect / MemHeapGetCurrentId (440490) */
s32 memNextHeapId = 1;          /* id of the next heap made (440268, 440320) */
/* zeros up to the texture descriptors, no label: the end of the heap
 * module's .data or a gap the original alignment left (unknown) */
static s32 memUnused[14] = { 0 };

/* Texture descriptor (gph.h GphTex) */
typedef struct {
    u32 w;
    u32 h;
    u32 tw;                     /* log2 width (TEX0 TW) */
    u32 th;
    u32 psm;
    u32 unk14;
    u32 size;                   /* bytes */
    void *data[7];
    s32 unk38;
    s32 unk3C;
    void *unk40[8];
    s64 id;
    u8 pad68[4];
    s32 uploadMode;
    u8 pad70[0xC];
    s32 texId;
} GphTex; /* size 0x80 */

/* filled by every frame-buffer effect before TEX0 */
GphTex gphFbTexInfo = { 0 };

/* image uploads in strips (444270, 444628): 640x112, PSMCT32 */
GphTex gphFbUploadTexInfo = {
    640, 112, 10, 7, 0, 0, 0x46000, { (void *)0x5000000 },
};

/* ripple (447C20, built by 448068) */
extern u8 gphRippleGridBuf[];         /* .bss: 33x33 vertex grid */
extern u8 gphRipplePacketBuf[];         /* .bss: draw packet buffer */
void *gphRippleGrid = gphRippleGridBuf;
void *gphRipplePacket = gphRipplePacketBuf;

s32 gphScanlineY = 0;           /* 4475B8 */

/* particle strip draw object (PtclInit 44A1F8, ptclDrawCtx) */
FVECTOR ptclDefaultColor = { 127.0f, 127.0f, 127.0f, 127.0f }; /* ambient */

/* VU light block (gph.h GphLightVuBlock, see GphLightVuBlockReset 432018)
 * and zeros up to the next file */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u32 vifCode;                /* VIF UNPACK V4-32 */
    FVECTOR unk10[8];
    FVECTOR dirColor[4];
    FVECTOR pointColor[4];
    FVECTOR dirVec[4];
    FVECTOR pointPos[4];        /* w = 1 / range */
    u8 pad190[0x50];
} PtclLightBlock; /* size 0x1E0 */

/* ptclDrawCtx.lightBlock: four coloured directional lights */
PtclLightBlock ptclLightBlock = {
    0, 0, 0, 0x6C100180,
    { { 0 } },
    {
        { 255.0f, 0.0f, 0.0f, 0.0f },
        { 255.0f, 255.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 255.0f, 0.0f },
        { 255.0f, 0.0f, 255.0f, 0.0f },
    },
    { { 0 } },
    {
        { 0.0f, -1.0f, 0.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
    },
    {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
    },
};
