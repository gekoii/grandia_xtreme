/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    u64 SBP : 14;
    u64 pad14 : 2;
    u64 SBW : 6;
    u64 pad22 : 2;
    u64 SPSM : 6;
    u64 pad30 : 2;
    u64 DBP : 14;
    u64 pad46 : 2;
    u64 DBW : 6;
    u64 pad54 : 2;
    u64 DPSM : 6;
    u64 pad62 : 2;
} GsBitbltbuf;

typedef struct {
    u64 RRW : 12;
    u64 pad12 : 20;
    u64 RRH : 12;
    u64 pad44 : 20;
} GsTrxreg;

typedef struct {
    u128 vifcode;
    u128 giftag;
    GsBitbltbuf bitbltbuf;
    u64 bitbltbufaddr;
    u64 trxpos;
    u64 trxposaddr;
    GsTrxreg trxreg;
    u64 trxregaddr;
    u64 finish;
    u64 finishaddr;
    u64 trxdir;
    u64 trxdiraddr;
} GsStoreImage;

#define D1_CHCR ((volatile u32 *)0x10009000)
#define D1_MADR ((volatile u32 *)0x10009010)
#define D1_QWC ((volatile u32 *)0x10009020)
#define GIF_CTRL ((volatile u32 *)0x10003000)
#define VIF1_STAT ((volatile u32 *)0x10003C00)
#define VIF1_FBRST ((volatile u32 *)0x10003C10)
#define VIF1_FIFO ((volatile u128 *)0x10005000)
#define GS_CSR ((volatile u64 *)0x12001000)
#define GS_BUSDIR ((volatile u64 *)0x12001040)

u64 GsGetIMR(void);
u64 GsPutIMR(u64);
extern const u128 _sceGsMskPath3Clear;
extern char _sceGsStoreImageDmaTimeoutStr[];
extern char _sceGsStoreImageGsTimeoutStr[];
extern char _sceGsStoreImageRecvTimeoutStr[];
extern char _sceGsStoreImageVif1TimeoutStr[];

s32 sceGsExecStoreImage(GsStoreImage *sp, u128 *dstaddr) {
    union { u128 q; u8 b[16]; } tmp;
    s32 w;
    s32 h;
    s32 h2;
    s32 psm;
    s32 bytes;
    s32 rest;
    s32 qwc;
    s32 odd;
    s32 pad;
    s32 size;
    u64 imr;
    s32 i;
    u32 count;

    pad = 0;
    odd = 0;
    qwc = 0;
    rest = 0;
    count = 0;
    w = sp->trxreg.RRW;
    psm = sp->bitbltbuf.SPSM;
    h = sp->trxreg.RRH;
    h2 = 0;
    switch (psm) {
    case 0x00:
    case 0x30:
        bytes = w * h * 4;
        odd = bytes & 0xF;
        rest = (bytes >> 4) & 7;
        qwc = (bytes >> 4) & 0xFFFFFFF8;
        if (odd == 0) {
            h2 = h;
            pad = 0;
        } else {
            h2 = (h + 3) & 0x1FFC;
            size = (w * h2) >> 2;
            pad = size - qwc - rest - 1;
        }
        break;
    case 0x01:
    case 0x31:
        bytes = w * h * 3;
        odd = bytes & 0xF;
        rest = (bytes >> 4) & 7;
        qwc = (bytes >> 4) & 0xFFFFFFF8;
        if (odd == 0) {
            h2 = h;
            pad = 0;
        } else {
            h2 = (h + 15) & 0x1FF0;
            size = (w * h2 * 3) >> 4;
            pad = size - qwc - rest - 1;
        }
        break;
    case 0x02:
    case 0x0A:
    case 0x32:
    case 0x3A:
        bytes = w * h * 2;
        odd = bytes & 0xF;
        rest = (bytes >> 4) & 7;
        qwc = (bytes >> 4) & 0xFFFFFFF8;
        if (odd == 0) {
            h2 = h;
            pad = 0;
        } else {
            h2 = (h + 7) & 0xFFFFFFF8;
            size = (w * h2) >> 3;
            pad = size - qwc - rest - 1;
        }
        break;
    case 0x13:
    case 0x1B:
        bytes = w * h;
        odd = bytes & 0xF;
        rest = (bytes >> 4) & 7;
        qwc = (bytes >> 4) & 0xFFFFFFF8;
        if (odd == 0) {
            h2 = h;
            pad = 0;
        } else {
            h2 = (h + 7) & 0xFFFFFFF8;
            size = (w * h2) >> 4;
            pad = size - qwc - rest - 1;
        }
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        bytes = (w * h) >> 1;
        odd = bytes & 0xF;
        rest = (bytes >> 4) & 7;
        qwc = (bytes >> 4) & 0xFFFFFFF8;
        if (odd == 0) {
            h2 = h;
            pad = 0;
        } else {
            h2 = (h + 7) & 0xFFFFFFF8;
            size = (w * h2) >> 5;
            pad = size - qwc - rest - 1;
        }
        break;
    }
    if (odd) {
        *(u64 *)((u32)&sp->trxreg | 0x20000000) = (u64)(s32)sp->trxreg.RRW | ((u64)h2 << 32);
    }
    while (*D1_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            printf(_sceGsStoreImageDmaTimeoutStr);
            return -1;
        }
    }
    imr = GsPutIMR(GsGetIMR() | 0x200);
    *GS_CSR = 2;
    *D1_QWC = 7;
    if (((u32)sp & 0x70000000) == 0x70000000) {
        *D1_MADR = ((u32)sp & 0x0FFFFFFF) | 0x80000000;
    } else {
        *D1_MADR = (u32)sp & 0x0FFFFFFF;
    }
    *D1_CHCR = 0x101;
    while (*D1_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            printf(_sceGsStoreImageDmaTimeoutStr);
            return -1;
        }
    }
    while (!(*GS_CSR & 2)) {
        if (count++ > 0x1000000) {
            printf(_sceGsStoreImageGsTimeoutStr);
            *VIF1_FIFO = _sceGsMskPath3Clear;
            return -1;
        }
    }
    *VIF1_STAT = 0x800000;
    *GS_BUSDIR = 1;
    if (qwc) {
        *D1_QWC = qwc;
        if (((u32)dstaddr & 0x70000000) == 0x70000000) {
            *D1_MADR = ((u32)dstaddr & 0x0FFFFFFF) | 0x80000000;
        } else {
            *D1_MADR = (u32)dstaddr & 0x0FFFFFFF;
        }
        *D1_CHCR = 0x100;
        while (*D1_CHCR & 0x100) {
            if (count++ > 0x1000000) {
                printf(_sceGsStoreImageRecvTimeoutStr);
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
    }
    for (i = 0; i < rest; i++) {
        while (!(*VIF1_STAT & 0x1F000000)) {
            if (count++ > 0x1000000) {
                printf(_sceGsStoreImageVif1TimeoutStr);
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
        dstaddr[qwc + i] = *VIF1_FIFO;
    }
    if (odd) {
        while (!(*VIF1_STAT & 0x1F000000)) {
            if (count++ > 0x1000000) {
                printf(_sceGsStoreImageVif1TimeoutStr);
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
        tmp.q = *VIF1_FIFO;
        for (i = 0; i < odd; i++) {
            ((u8 *)&dstaddr[qwc + rest])[i] = tmp.b[i];
        }
        for (i = 0; i < pad; i++) {
            while (!(*VIF1_STAT & 0x1F000000)) {
                if (count++ > 0x1000000) {
                    printf(_sceGsStoreImageVif1TimeoutStr);
                    *GS_CSR = 0x100;
                    *GS_BUSDIR = 0;
                    *GIF_CTRL = 1;
                    *VIF1_FBRST = 1;
                    return -1;
                }
            }
            tmp.q = *VIF1_FIFO;
        }
    }
    *VIF1_STAT = 0;
    *GS_BUSDIR = 0;
    GsPutIMR(imr);
    *GS_CSR = 2;
    *VIF1_FIFO = _sceGsMskPath3Clear;
    return 0;
}
