/* Port-only C for GsVif1DmaWait (0x0045ECB0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"
#include "grx_dma.h"

void GsVif1DmaWait(void);

/* Waits for VIF1 DMA (D1_CHCR.STR) to stop; after 0x4000000 polls it
   gives up and jumps to GsDmaTimeoutHook. In the port DMA is synchronous
   (grx_dma.h): the channel is idle at once, unless it can't finish (the
   MFIFO drain, a failed transfer), which goes to the timeout hook at once
   instead of after the polls. */
void GsVif1DmaWait(void) {
    if (grx_dma_wait(GRX_DMA_VIF1) != 0) {
        GsDmaTimeoutHook();
    }
}
