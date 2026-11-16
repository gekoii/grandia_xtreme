/* main executable .data 0x0049B390-0x0049B880: the env-map GS packet, the
 * frame buffer addresses, the VIF init/flush packets and the sound IOP
 * command and stream state. Data-only C (plan step 6b, docs/shifting.md);
 * the users are GphEnvMapInit/GphEnvMapDrawLayer (43C1A8.c, 43C260.c), the
 * GS setup (42F6B0.c, 42FC60.c, 42FEE0.c), GsVifInit (4562A0.c),
 * GsVif1Flush/FlushE (456310.c) and the sound code (45F398.c..460380.c,
 * main/n10.h).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049B390-0x0049B880 */

typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef unsigned long long u64;

/* ---- env map (main/n05.h) ---- */
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

/* rewritten by GphEnvMapInit (PRMODE, TEX1_1, TEST_1, ALPHA_1) */
GsAdPkt gphEnvMapGsPkt = {
    0x10000006, 0,
    { 0, 0, 0 }, 0x50000005,
    0x1000400000008004ULL, 0xE,
    {
        { 0x78, GS_PRMODE },
        { 0x5140D, GS_TEST_1 },
        { 0, GS_CLAMP_1 },
        { 0x44, GS_ALPHA_1 },
    },
};

/* no label, no known user: a VIF UNPACK V4-8 of 0x80 colours to VU address
 * 0x100 at +0x30 (like the particle colour packets), then zeros */
static u32 gphEnvMapUnusedPacket[0xB0] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0x6E800100,
};

/* ---- GS frame buffers (gph.h) ---- */
u32 gsDispFrameAddr = 0;
u32 gsDrawFrameAddr = 0x46000;  /* GS word addresses: one 640x448 32-bit buffer each */
u32 gsZBufAddr = 0x8C000;

/* ---- VIF packets (GsVifInit 4562A0, GsVif1Flush/FlushE 456310) ---- */
#define VIF_NOP             0x00000000
#define VIF_STCYCL(wl, cl)  (0x01000000 | (wl) << 8 | (cl))
#define VIF_OFFSET(n)       (0x02000000 | (n))
#define VIF_BASE(n)         (0x03000000 | (n))
#define VIF_ITOP(n)         (0x04000000 | (n))
#define VIF_STMOD(n)        (0x05000000 | (n))
#define VIF_MSKPATH3(n)     (0x06000000 | (n))
#define VIF_FLUSHE          0x10000000
#define VIF_FLUSH           0x11000000
#define VIF_MSCAL(a)        (0x14000000 | (a))
#define VIF_MSCNT           0x17000000
#define VIF_STMASK          0x20000000
#define VIF_STROW           0x30000000
#define F_1                 0x3F800000 /* 1.0f */

/* VIF0, 1 qword */
u32 gsVif0InitPacket[4] = { VIF_STCYCL(1, 1), VIF_ITOP(0), VIF_STMOD(0), VIF_STMASK };

/* VIF1: GsVifInit sends the first 4 qwords; the 1-qword packets after them
 * have no label and no known user */
u32 gsVif1InitPacket[56] = {
    VIF_STCYCL(1, 1), VIF_OFFSET(0), VIF_BASE(0), VIF_ITOP(0),
    VIF_STMOD(0), VIF_MSKPATH3(1), VIF_STMASK, 0,
    VIF_STROW, F_1, F_1, F_1,
    F_1, VIF_NOP, VIF_NOP, VIF_NOP,
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_MSCAL(0),
    VIF_NOP, VIF_NOP, VIF_MSCAL(0), VIF_FLUSH,
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_MSCAL(2),
    VIF_NOP, VIF_NOP, VIF_MSCAL(2), VIF_FLUSHE,
    VIF_NOP, VIF_MSCAL(2), VIF_FLUSH, VIF_MSCNT,
    VIF_FLUSH, VIF_MSCNT, VIF_FLUSHE, VIF_NOP,
    VIF_MSCAL(2), VIF_FLUSH, VIF_MSCNT, VIF_FLUSHE,
    VIF_MSCAL(4), VIF_FLUSH, VIF_MSCNT, VIF_FLUSHE,
    VIF_NOP, VIF_NOP, VIF_MSCNT, VIF_FLUSHE,
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_MSCNT,
};

u32 gsVif1FlushPacket[4] = { VIF_NOP, VIF_NOP, VIF_NOP, VIF_FLUSH };

/* GsVif1FlushE sends the first qword; the rest has no label */
u32 gsVif1FlushEPacket[16] = {
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_FLUSHE,
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_MSCAL(6),
    VIF_NOP, VIF_NOP, VIF_NOP, VIF_MSCAL(8),
    VIF_FLUSHE, VIF_MSCNT, VIF_FLUSHE, VIF_NOP,
};

/* ---- sound IOP commands and streams (main/n10.h) ---- */
s8 sndIopStatusRecvBusy = 0;
s8 sndMovieStreaming = 1;
s8 sndIopCmdDmaBusy = 0;
s8 sndIopStringCmdPending = 0;
s16 sndIopCmdSeq = 1;           /* never 0 (SndIopCmdPush 45F398) */
s16 sndIopCmdDmaId = 0;
s32 sndIopCmdCount = 0;         /* queued commands, up to 0x18 */
/* zeros, no label: unused variables (unknown) */
static s32 sndUnused0 = 0;
static s32 sndUnused1[2] = { 0, 0 };
s32 sndIopCmdLock = 0;          /* a word here; 45F398 reads its low byte (s8) */
s8 sndIopRecvBusy = 0;
s8 sndStrmFetchWrapped = 0;
s32 sndStrmFetchSlot = -16;
s16 sndIopWaitSeq = 0;
s16 sndStrmUploadSeq = -1;
s16 sndStrmUploadDmaId = 0;
s32 sndStrmUploadSlot = -1;
s32 sndStrmUploadCount = -1;
s32 sndStrmUploadRemain = -1;
s32 sndStrmUploadLastOfs = -1;  /* SndStrmUploadStart (460380) */
s32 sndFsDirBufOfs = 0;         /* 460670 */
/* zeros up to the pan tables, no label (unknown) */
static s32 sndUnused2[8] = { 0 };
