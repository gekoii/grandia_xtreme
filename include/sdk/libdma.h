/*
 * Sony libdma types (SCE libdma.h / eestruct.h names): the channel CHCR
 * bitfield, one channel's registers and the D_CTRL/D_PCR/D_SQWC/D_RBOR/D_RBSR
 * settings. sceDmaReset (0x0046E098) keeps a local view (reads chcr as a
 * volatile u32, as0/as1 swapped).
 */
#ifndef SDK_LIBDMA_H
#define SDK_LIBDMA_H

#include "common.h"

#ifndef SCE_DMA_TYPES
#define SCE_DMA_TYPES
typedef struct tD_CHCR {
    u32 DIR : 1;
    u32 p0 : 1;
    u32 MOD : 2;
    u32 ASP : 2;
    u32 TTE : 1;
    u32 TIE : 1;
    u32 STR : 1;
    u32 p1 : 7;
    u32 TAG : 16;
} tD_CHCR;

/* One channel's registers (sceDmaGetChan). */
typedef struct sceDmaChan {
    /* 0x00 */ tD_CHCR chcr;
    /* 0x04 */ u32 p0[3];
    /* 0x10 */ void *madr;
    /* 0x14 */ u32 p1[3];
    /* 0x20 */ u32 qwc;
    /* 0x24 */ u32 p2[3];
    /* 0x30 */ void *tadr;
    /* 0x34 */ u32 p3[3];
    /* 0x40 */ void *as0;
    /* 0x44 */ u32 p4[3];
    /* 0x50 */ void *as1;
    /* 0x54 */ u32 p5[3];
    /* 0x60 */ u32 p6[4];
    /* 0x70 */ u32 p7[4];
    /* 0x80 */ void *sadr;
    /* 0x84 */ u32 p8[3];
} sceDmaChan; /* size 0x90 */

/* D_CTRL/D_PCR/D_SQWC/D_RBOR/D_RBSR settings (sceDmaPutEnv). */
typedef struct sceDmaEnv {
    /* 0x00 */ u8 sts;
    /* 0x01 */ u8 std;
    /* 0x02 */ u8 mfd;
    /* 0x03 */ u8 rcycle;
    /* 0x04 */ u16 express;
    /* 0x06 */ u16 notify;
    /* 0x08 */ u16 sqwc;
    /* 0x0A */ u16 tqwc;
    /* 0x0C */ void *rbadr;
    /* 0x10 */ u32 rbmsk;          /* ring size - 1, qword aligned */
} sceDmaEnv; /* size 0x14 */
#endif

#endif
