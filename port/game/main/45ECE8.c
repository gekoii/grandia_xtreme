/* Port-only C for GsVif1DmaSend (0x0045ECE8), which the decompilation still has in assembly. */
#include "common.h"
#include "grx_dma.h"

#define D1_MADR (*(volatile u32 *)0x10009010)
#define D1_QWC (*(volatile u32 *)0x10009020)

void GsVif1DmaSend(void *, u32);

/* Starts a normal-mode VIF1 DMA of qwc quadwords from addr. A scratchpad
   address (0x70000000 and up, compared as a signed difference) gets
   0x10000000 added, which sets the MADR SPR bit. The CHCR store (normal
   mode, from memory, STR) runs the transfer at once (grx_dma.h). */
void GsVif1DmaSend(void *addr, u32 qwc) {
    s32 a = (s32)addr;

    if (a - 0x70000000 >= 0) {
        a += 0x10000000;
    }
    D1_MADR = a;
    D1_QWC = qwc;
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x101);
}
