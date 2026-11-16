/* Port-only C for sceGsDefDispEnv (0x0046D188), which the decompilation still has in assembly. */
/* Sony PS2 SDK library code (libgraph): see README.md, "Third-party library code". */
#include "common.h"

/* sceGsGParam (_sceGsGParam, port/game/main/data_49DD70.c), as
   sceGsResetGraph (src/main/46CF80.c) fills it */
typedef struct {
    s16 interMode; /* 0 non-interlaced, 1 interlaced */
    s16 outMode;   /* 2 NTSC, 3 PAL */
    s16 ffMode;    /* 0 field, 1 frame */
    s16 version;
} GsGParam;

extern GsGParam *sceGsGetGParam(void);
s32 printf(const char *, ...);

/* _sceGsDefDispEnvModeFmt (0x004A6C60) */
static const char modeFmt[] = "sceGsDefDispEnv:Not support displaymode for %d!!\n";

/* The callers' structs (GsDispEnv in include/gph.h, MovDisp in
   include/main/n11.h) both have the five registers as 64-bit values at
   0x00..0x20; MovDisp's are u32 pairs, 4-aligned. */
static void put64(void *env, int off, u64 v) {
    __builtin_memcpy((u8 *)env + off, &v, 8);
}

/* DISPLAY: DX (0-11), DY (12-22), MAGH (23-26), MAGV (27-28) = 0,
   DW (32-43), DH (44-54). As the asm builds it: DW and DH are the 32-bit
   values shifted without a mask, MAGH the sign-extended one. */
static u64 display(s32 w, s32 dx, s32 dy, s32 dh, s32 dxBase, s32 dyBase) {
    s32 magh = (w + 0x9FF) / w; /* 2560 / w rounded up: VCK cycles per pixel */
    u64 v = (u64)((s64)(dx * magh) + dxBase) & 0xFFF;

    v |= (u64)((dy + dyBase) & 0xFFF) << 12;
    v |= (u64)(s64)(magh - 1) << 23;
    v |= (u64)(u32)(magh * w - 1) << 32;
    v |= (u64)(u32)(dh - 1) << 44;
    return v;
}

/* Fills a display environment (PMODE, SMODE2, DISPFB2, DISPLAY2, BGCOLOR)
   for a w x h frame buffer of format psm at block 0, placed dx, dy from
   the standard origin of the CRT mode sceGsResetGraph set. The arguments
   are shorts (the asm sign-extends each from 16 bits). An output mode
   other than NTSC or PAL leaves DISPLAY as it was and prints. */
void sceGsDefDispEnv(void *env, s32 psm_, s32 w_, s32 h_, s32 dx_, s32 dy_) {
    s16 psm = (s16)psm_, w = (s16)w_, h = (s16)h_, dx = (s16)dx_, dy = (s16)dy_;
    GsGParam *gp = sceGsGetGParam();
    s32 inter, dh;

    /* PMODE: EN1 0, EN2 1, CRTMD 1, MMOD 1 (ALP), AMOD 1, SLBG 0, ALP 0 */
    put64(env, 0x00, 0x66);
    /* SMODE2: INT, FFMD (FFMD 1 when not interlaced), DPMS 0 */
    put64(env, 0x08, gp->interMode == 0 ? 2 : gp->ffMode != 0 ? 3 : 1);
    /* DISPFB: FBP 0, FBW, PSM, DBX 0, DBY 0 */
    put64(env, 0x10, ((u64)(psm & 0xF) << 15) | ((u64)(((w + 63) >> 6) & 0x3F) << 9));

    inter = gp->interMode == 1;
    /* DH: in interlaced frame mode the h lines are read for each field, so
       they cover 2h lines of the interlaced picture */
    dh = (inter && gp->ffMode != 0) ? h * 2 : h;
    switch (gp->outMode) {
    case 2: /* NTSC */
        put64(env, 0x18, display(w, dx, dy, dh, 0x27C, inter ? 0x32 : 0x19));
        break;
    case 3: /* PAL */
        put64(env, 0x18, display(w, dx, dy, dh, 0x290, inter ? 0x48 : 0x24));
        break;
    default:
        printf(modeFmt, gp->outMode);
        break;
    }
    /* BGCOLOR: black */
    put64(env, 0x20, 0);
}
