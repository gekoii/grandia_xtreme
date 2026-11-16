/* Port-only C for data the decompilation still has in assembly (asm/main/data/main/09DD70.data.s). */
#include "common.h"

typedef struct SceMpegPicture SceMpegPicture; /* include/sdk/libmpeg.h (0x68) */
/* include/sdk/libmpeg.h */
typedef struct SceMpegStrType {
    u64 pattern;
    u64 mask;
} SceMpegStrType;
typedef void (*ExtFunc)(void);

void _copyrightExtension(void);
void _pictureCodingExtension(void);
void _pictureDisplayExtension(void);
void _pictureSpatialScalableExtension(void);
void _pictureTemporalScalableExtension(void);
void _quantMatrixExtension(void);
extern u32 _sceMpegPicBottom0[];
extern u32 _sceMpegPicBottom1[];
extern u32 _sceMpegPicBottom2[];
extern u32 _sceMpegPicFrame0[];
extern u32 _sceMpegPicFrame1[];
extern u32 _sceMpegPicFrame2[];
extern u32 _sceMpegPicTop0[];
extern u32 _sceMpegPicTop1[];
extern u32 _sceMpegPicTop2[];
extern u8 _sceMpegRefDmaTagBuf[];
extern const char _scePadEmptyStr[];
extern const char _scePadReqStrBusy[];
extern const char _scePadReqStrComplete[];
extern const char _scePadReqStrFailed[];
extern const char _scePadStateStrDisconnect[];
extern const char _scePadStateStrError[];
extern const char _scePadStateStrExecCmd[];
extern const char _scePadStateStrFindCtp1[];
extern const char _scePadStateStrStable[];
void _sequenceDisplayExtension(void);
void _sequenceExtension(void);
void _sequenceScalableExtension(void);
void _unknownExtension(void);

/* sceGsGParam (M2C_UNK in src/main/46D110.c): modes, then the VSync callback and its id */
struct {
    s16 interMode;
    s16 outMode;
    s16 ffMode;
    s16 version;
    void *vscFunc;
    s32 vscId;
} _sceGsGParam = {1, 2, 1, 3, 0, 0};

/* const u128 in src (one quadword) */
const u32 _sceGsMskPath3Clear[4] __attribute__((aligned(16))) = {
    0x6000000, 0, 0, 0,
};

/* DMA channel registers (sceDmaChan * / DmaCh * in src) */
volatile void *_sceDmaChanTable[10] = {
    (volatile void *)0x10008000, (volatile void *)0x10009000, (volatile void *)0x1000A000, (volatile void *)0x1000B000,
    (volatile void *)0x1000B400, (volatile void *)0x1000C000, (volatile void *)0x1000C400, (volatile void *)0x1000C800,
    (volatile void *)0x1000D000, (volatile void *)0x1000D400,
};

/* s32 in src, then up to the next label: padding and "PsIIlibdma  2300" */
struct {
    s32 v;
    char tail[0x14];
} _sceDmaDebugMode = {
    0,
    "\000\000\000\000PsIIlibdma  2300",
};

s32 _sceDmaChanHasTag[10] = {
    1, 1, 1, 1, 1, 0, 0, 0,
    1, 1,
};

u8 _sceDmaStsTable[16] = {
    0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 _sceDmaStdTable[16] = {
    0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 _sceDmaMfdTable[16] = {
    0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* sceDmaEnv (0x14) in src, all zero here, then up to the next label: padding and "PsIIlibcdvd 2340" */
struct {
    s32 v[5];
    char tail[0x24];
} _sceDmaEnv = {
    {
        0, 0, 0, 0, 0,
    },
    "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000PsIIlibcdvd 2340",
};

volatile s32 _sceCdCbSema = -1;

volatile s32 _sceCdNCmdSema = -1;

volatile s32 _sceCdSCmdSema = -1;

s32 _sceCdNCmdBind = -1;

s32 _sceCdSifCmdBind = -1;

s32 _sceCdSearchFileBind = -1;

s32 _sceCdDiskReadyBind = -1;

s32 _sceCdSCmdBind = -1;

s32 _sceCdInitBind = -1;

/* volatile s32 in src, then up to the next label: zeros */
struct {
    volatile s32 v;
    u8 tail[0x24];
} sceCdCbfunc_num = {
    0,
};

s32 _sceCdNCmdRecvBuf[0x454] __attribute__((aligned(16))) = {0};

/* sceSifClientData (0x28) in src; all zero */
u32 _sceCdNCmdClient[0xC] __attribute__((aligned(4))) = {0};

s32 _sceCdSCmdRecvBuf[0x110] __attribute__((aligned(16))) = {0};

s32 _sceCdSCmdSendBuf[0x102] __attribute__((aligned(16))) = {0};

/* sceSifClientData (0x28) in src, all zero here, then up to the next label: padding and "PsIIlibmc   2320" */
struct {
    u32 v[10];
    char tail[0x10];
} _sceCdSCmdClient = {
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0,
    },
    "PsIIlibmc   2320",
};

s32 _sceMcSema = -1;

SceMpegStrType _sceMpegStrTypeTable[10] = {
    {0xE000000000, 0xFF00000000}, {0xBDFFC00000, 0xFFFFFFFFFF},
    {0xBDFFA00000, 0xFFFFFFFFFF}, {0xBDFFA10000, 0xFFFFFFFFFF},
    {0xBDFF900000, 0xFFFFFFFFFF}, {0xC000000000, 0xFF00000000},
    {0xBD80000000, 0xFFFF000000}, {0xBDA0000000, 0xFFFF000000},
    {0xBD88000000, 0xFFFF000000}, {0xBD90000000, 0xFFFF000000},
};

/* u8[16] in src, then up to the next label: padding and "PsIIlibmpeg 2300" */
struct {
    u8 v[16];
    char tail[0x10];
} _sceMpegPesFlagBits = {
    {
        0x00, 0x10, 0x08, 0x18, 0x08, 0x18, 0x10, 0x20, 0x18, 0x28, 0x20, 0x30, 0x20, 0x30, 0x28, 0x38,
    },
    "PsIIlibmpeg 2300",
};

/* scratchpad address */
u8 *_sceMpegSprWork = (u8 *)0x70000000;

/* u32 in libmpeg.h (s32 in 473B70.c): the tag buffer's address. An
   address cut to 32 bits isn't a constant on x86-64: set at start-up
   there (it fits: the image is below 2 GB, port/docs/win64.md) */
#if __SIZEOF_POINTER__ == 4
u32 _sceMpegRefDmaTags = (u32)_sceMpegRefDmaTagBuf;
#else
u32 _sceMpegRefDmaTags;
static void __attribute__((constructor)) grx_mpeg_ref_dma_tags(void) {
    _sceMpegRefDmaTags = (u32)(__UINTPTR_TYPE__)_sceMpegRefDmaTagBuf;
}
#endif

/* scratchpad address */
u32 _sceMpegSprPred = 0x70003600;

/* u32 in src, then up to the next label: zeros */
struct {
    u32 v;
    u8 tail[0x24];
} _sceMpegInputBuf = {
    0,
};

SceMpegPicture *_sceMpegFwdRefFrame = (SceMpegPicture *)_sceMpegPicFrame0;

SceMpegPicture *_sceMpegBwdRefFrame = (SceMpegPicture *)_sceMpegPicFrame1;

SceMpegPicture *_sceMpegAuxFrame = (SceMpegPicture *)_sceMpegPicFrame2;

SceMpegPicture *_sceMpegFwdRefTop = (SceMpegPicture *)_sceMpegPicTop0;

SceMpegPicture *_sceMpegBwdRefTop = (SceMpegPicture *)_sceMpegPicTop1;

SceMpegPicture *_sceMpegAuxTop = (SceMpegPicture *)_sceMpegPicTop2;

SceMpegPicture *_sceMpegFwdRefBottom = (SceMpegPicture *)_sceMpegPicBottom0;

SceMpegPicture *_sceMpegBwdRefBottom = (SceMpegPicture *)_sceMpegPicBottom1;

SceMpegPicture *_sceMpegAuxBottom = (SceMpegPicture *)_sceMpegPicBottom2;

/* s32 in src, then up to the next label: IPU and DMA register addresses that only asm uses (no labels) */
struct {
    s32 v;
    u32 tail[22];
} _sceMpegBitsStale = {
    1,
    {
        0, 0x10002000, 0x10002010, 0x10002020,
        0x10002030, 0x1000B430, 0x1000B410, 0x1000B400,
        0x1000B420, 0x1000B010, 0x1000B000, 0x1000B020,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0,
    },
};

u8 default_intra_quantizer_matrix[64] = {
    0x08, 0x10, 0x10, 0x13, 0x10, 0x13, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x1A, 0x18, 0x1A, 0x1B,
    0x1B, 0x1B, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1D, 0x1D, 0x1D, 0x22, 0x22, 0x22, 0x1D,
    0x1D, 0x1D, 0x1B, 0x1B, 0x1D, 0x1D, 0x20, 0x20, 0x22, 0x22, 0x25, 0x26, 0x25, 0x23, 0x23, 0x22,
    0x23, 0x26, 0x26, 0x28, 0x28, 0x28, 0x30, 0x30, 0x2E, 0x2E, 0x38, 0x38, 0x3A, 0x45, 0x45, 0x53,
};

u8 _sceMpegDefNonIntraQuantMatrix[64] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicFrame0[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicFrame1[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicFrame2[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicTop0[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicTop1[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicTop2[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicBottom0[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicBottom1[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegPicture (0x68) in src; all zero */
u32 _sceMpegPicBottom2[0x1A] __attribute__((aligned(8))) = {0};

/* SceMpegMbSlots (0x288) in src; all zero */
u32 _sceMpegMbSlots[0xA2] __attribute__((aligned(8))) = {0};

/* libmpeg.h declares 16 elements; the asm object has 10 */
s32 _sceMpegIpuCmdInvalidatesBfr[10] = {
    1, 1, 0, 0, 0, 1, 1, 1,
    1, 1,
};

s32 True_Framenum_max = -1;

ExtFunc _sceMpegExtensionTable[11] = {
    _unknownExtension, _sequenceExtension, _sequenceDisplayExtension, _quantMatrixExtension,
    _copyrightExtension, _sequenceScalableExtension, _unknownExtension, _pictureDisplayExtension,
    _pictureCodingExtension, _pictureSpatialScalableExtension, _pictureTemporalScalableExtension,
};

u32 _sceMpegFieldCountTable[16] = {
    2, 0, 2, 0, 2, 3, 2, 3,
    0, 0, 0, 0, 2, 4, 0, 6,
};

/* volatile s32 in src, then up to the next label: padding and "PsIIlibipu  2300" */
struct {
    volatile s32 v;
    char tail[0x1C];
} _sceMpegCscChunkDone = {
    0,
    "\000\000\000\000\000\000\000\000\000\000\000\000PsIIlibipu  2300",
};

/* volatile u128[5] in src */
u32 _sceIpuInitIqMatrix[20] __attribute__((aligned(16))) = {
    0x13101008, 0x16161310, 0x16161616, 0x1B1A181A, 0x1A1A1B1B, 0x1B1B1A1A, 0x1D1D1D1B, 0x1D222222,
    0x1B1B1D1D, 0x20201D1D, 0x26252222, 0x22232325, 0x28262623, 0x30302828, 0x38382E2E, 0x5345453A,
    0x10101010, 0x10101010, 0x10101010, 0x10101010,
};

/* u128[] in src: the CLUT (2 quadwords), then the libipu version string */
u32 _sceIpuInitVqClut[12] __attribute__((aligned(16))) = {
    0x4210000, 0x3E00842, 0x14A51084, 0x1CE718C6, 0x2529001F, 0x7C00294A, 0x35AD318C, 0x39CE7FFF,
    0x49497350, 0x7062696C, 0x20206461, 0x30303332,
};

s32 _scePadDebugPrint = 1;

char *_scePadStateStrTable[8] = {
    (char *)_scePadStateStrDisconnect, (char *)_scePadEmptyStr, (char *)_scePadStateStrFindCtp1, (char *)_scePadEmptyStr,
    (char *)_scePadEmptyStr, (char *)_scePadStateStrExecCmd, (char *)_scePadStateStrStable, (char *)_scePadStateStrError,
};

char *_scePadReqStateStrTable[3] = {
    (char *)_scePadReqStrComplete, (char *)_scePadReqStrFailed, (char *)_scePadReqStrBusy,
};
