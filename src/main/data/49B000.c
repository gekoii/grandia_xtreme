/* main executable .data 0x0049B000-0x0049B390: the particle emitters' UV
 * packet, the slide wipe table, memory card polling state, the shadow colour
 * and the default GS register packets. Data-only C (plan step 6b,
 * docs/shifting.md); the users are PtclEmitterSysInit (450648.c),
 * GphWipeSlide (451CA8.c), the memory card code (main/n08.h), 43A450.c/
 * 43A5D8.c (shadows) and GphDrawListRender (436740.c, 4369F0.c).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049B000-0x0049B390 */

typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* ---- particle emitters (ptcl.h) ---- */
/* UV packet of the emitter draw template: VIF STMASK + UNPACK V2-16 (masked)
 * of 0x7C UVs to VU address 0x80, then 31 quads' corner UVs, then zeros */
typedef struct {
    u32 vif[4];
    s16 uv[31][8];
    u8 pad200[0x20];
} PtclUvPacket; /* size 0x220 */

#define QUAD_UV { 0, 0, -1024, 0, -1024, 1024, 0, 1024 }

PtclUvPacket ptclEmitterUvBlock = {
    { 0x20000000, 0x10101010, 0, 0x757C0080 },
    {
        QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV,
        QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV,
        QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV,
        QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV, QUAD_UV,
    },
};

/* ---- screen wipes (main/n07.h) ---- */
/* GphWipeSlide (451CA8): x = start + frame * 20 * dir, by mode bit 0 * 2 +
 * dir bit 0 */
typedef struct {
    f32 dir;
    f32 start;
} GphWipeSlide;

GphWipeSlide gphWipeSlideTable[4] = {
    { 1.0f, -640.0f },
    { -1.0f, 640.0f },
    { 1.0f, 0.0f },
    { -1.0f, 0.0f },
};

/* ---- memory card polling (main/n08.h) ---- */
s32 mcBusy = 1;
s32 mcSyncPort = 0;
s32 mcPollFrames = 0;
s32 mcBusyFrames = 0;
s32 mcScanBusyFrames = 0;
/* zeros up to gphShadowColor, no label: the end of the module's .data or a
 * gap the original alignment left (unknown) */
static s32 mcUnused[10] = { 0 };

/* ---- shadows (main/n05.h) ---- */
FVECTOR gphShadowColor = { 0.0f, 0.0f, 0.0f, 0.0f }; /* set by 43A370 */

/* ---- draw list (gph.h) ---- */
/* GS A+D register packet (GsAdPkt, laid out by GsAdPktInit 43B9E0): DMA
 * cnt tag, VIF NOP x3 + DIRECT, GIF tag (A+D), then the registers. A DMA
 * source, so 16-aligned. */
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
#define GS_ZBUF_1       0x4E

/* GphDrawListRender (436740) copies [0] to the scratchpad and sends it
 * before the draw list; [1] (ZBUF) has no known user */
GsAdPkt gphDefaultGsRegsPacket[2] = {
    {
        0x10000006, 0,
        { 0, 0, 0 }, 0x50000005,
        0x1000400000008004ULL, 0xE,
        {
            { 0x78, GS_PRMODE },
            { 0x7140D, GS_TEST_1 },
            { 0, GS_CLAMP_1 },
            { 0x44, GS_ALPHA_1 },
        },
    },
    {
        0x10000003, 0,
        { 0, 0, 0 }, 0x50000002,
        0x1000400000008001ULL, 0xE,
        {
            { 0x100000000ULL, GS_ZBUF_1 },
        },
    },
};
