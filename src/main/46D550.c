/* rodata: 0x004A6C98-0x004A6E28 */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


#define D1_CHCR (*(volatile u32 *)0x10009000)
#define D1_MADR (*(volatile u32 *)0x10009010)
#define D1_QWC (*(volatile u32 *)0x10009020)
#define D1_TADR (*(volatile u32 *)0x10009030)
#define D2_CHCR (*(volatile u32 *)0x1000A000)
#define D2_MADR (*(volatile u32 *)0x1000A010)
#define D2_QWC (*(volatile u32 *)0x1000A020)
#define D2_TADR (*(volatile u32 *)0x1000A030)
#define VIF1_STAT (*(volatile u32 *)0x10003C00)
#define GIF_STAT (*(volatile u32 *)0x10003020)

#define DUMP \
    { \
        printf("\t<D1_CHCR=%08x:", D1_CHCR); \
        printf("D1_TADR=%08x:", D1_TADR); \
        printf("D1_MADR=%08x:", D1_MADR); \
        printf("D1_QWC=%08x>\r\n", D1_QWC); \
        printf("\t<D2_CHCR=%08x:", D2_CHCR); \
        printf("D2_TADR=%08x:", D2_TADR); \
        printf("D2_MADR=%08x:", D2_MADR); \
        printf("D2_QWC=%08x>\r\n", D2_QWC); \
        printf("\t<VIF1_STAT=%08x:", VIF1_STAT); \
        printf("GIF_STAT=%08x>\r\n", GIF_STAT); \
    }

#define CFC2_VI29(x) __asm__ volatile("cfc2.ni %0, $vi29" : "=r"(x))

s32 sceGsSyncPath(s32 mode, s32 timeout)
{
    u32 cnt;
    u32 stat;
    s32 ret;

    cnt = 0;
    if (mode == 0) {
        while (D1_CHCR & 0x100) {
            if (cnt++ > 0x1000000) {
                printf("sceGsSyncPath: DMA Ch.1 does not terminate\r\n");
                DUMP
                return -1;
            }
        }
        while (D2_CHCR & 0x100) {
            if (cnt++ > 0x1000000) {
                printf("sceGsSyncPath: DMA Ch.2 does not terminate\r\n");
                DUMP
                return -1;
            }
        }
        while (VIF1_STAT & 0x1F000003) {
            if (cnt++ > 0x1000000) {
                printf("sceGsSyncPath: VIF1 does not terminate\r\n");
                DUMP
                return -1;
            }
        }
        CFC2_VI29(stat);
        while (stat & 0x100) {
            if (cnt++ > 0x1000000) {
                printf("sceGsSyncPath: VU1 does not terminate\r\n");
                DUMP
                return -1;
            }
            CFC2_VI29(stat);
        }
        while (GIF_STAT & 0xC00) {
            if (cnt++ > 0x1000000) {
                printf("sceGsSyncPath: GIF does not terminate\r\n");
                DUMP
                return -1;
            }
        }
        return 0;
    }
    ret = 0;
    if (D1_CHCR & 0x100) {
        ret |= 1;
    }
    if (D2_CHCR & 0x100) {
        ret |= 2;
    }
    if (VIF1_STAT & 0x1F000003) {
        ret |= 4;
    }
    CFC2_VI29(stat);
    if (stat & 0x100) {
        ret |= 8;
    }
    if (GIF_STAT & 0xC00) {
        ret |= 0x10;
    }
    return ret;
}
