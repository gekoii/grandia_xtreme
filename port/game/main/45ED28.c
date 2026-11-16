/* Port-only C for GsVif1DmaSendChain (0x0045ED28), which the decompilation still has in assembly. */
#include "common.h"
#include "grx_dma.h"

#define D1_QWC (*(volatile u32 *)0x10009020)
#define D1_TADR (*(volatile u32 *)0x10009030)

void GsVif1DmaSendChain(void *tag);

/* Starts a source-chain VIF1 DMA at the tag. A scratchpad address
   (0x70000000 and up, compared as a signed difference, as GsVif1DmaSend
   does) gets 0x10000000 added, which sets the TADR SPR bit. CHCR 0x105:
   chain mode, from memory, STR, no TTE; the transfer runs at once
   (grx_dma.h). */
void GsVif1DmaSendChain(void *tag) {
    s32 a = (s32)tag;

    if (a - 0x70000000 >= 0) {
        a += 0x10000000;
    }
    D1_TADR = a;
    D1_QWC = 0;
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x105);
}
