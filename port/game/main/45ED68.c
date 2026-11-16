/* Port-only C for GsMfifoSend (0x0045ED68), which the decompilation still has in assembly. */
#include "common.h"
#include "grx_dma.h"

#define D8_CHCR (*(volatile u32 *)0x1000D000) /* fromSPR */
#define D8_MADR (*(volatile u32 *)0x1000D010)
#define D8_QWC (*(volatile u32 *)0x1000D020)
#define D8_SADR (*(volatile u32 *)0x1000D080)
#define D1_TADR (*(volatile u32 *)0x10009030) /* VIF1 */
#define D_RBSR (*(volatile u32 *)0x1000E040)

/* The scratchpad bounce buffer for packets in main memory: 0x70000A00 and
   0x70001800 in turn */
#define SPR_BOUNCE (*(u32 *)0x700000F0)

void GsMfifoSend(void *pkt, s32 qwc);

/* Sends qwc quadwords to the MFIFO ring with a fromSPR DMA. A packet below
   0x700000F0 (a signed difference) is first copied to the current bounce
   buffer, and the bounce buffers swap. Waits for the channel to be idle,
   then for the ring to have room for the packet (or to be empty), then
   starts the transfer: in the port it runs at once, and the MFIFO's VIF1
   drains the ring at once too (grx_dma.h), so both waits pass at once. */
void GsMfifoSend(void *pkt, s32 qwc) {
    /* Only the low 32 bits, as on the EE: some callers declare the packet
       an s32 (456718, 456B78), which on x86-64 leaves the register's upper
       half undefined; the game's memory is below 2 GB (port/docs/win64.md) */
    u32 addr = (u32)(__UINTPTR_TYPE__)pkt;
    u32 madr, tadr;
    GrxQuad *s, *d, *end;

    if ((s32)(addr - 0x700000F0) < 0) {
        s = (GrxQuad *)(__UINTPTR_TYPE__)addr;
        d = (GrxQuad *)SPR_BOUNCE;
        end = s + qwc;
        do {
            __builtin_memcpy(d, s, 16);
            s++;
            d++;
        } while (s != end);
        addr = SPR_BOUNCE;
        SPR_BOUNCE = addr != 0x70000A00 ? 0x70000A00 : 0x70001800;
    }
    while (D8_CHCR & 0x100) {
    }
    D8_SADR = addr + 0x10000000;
    D8_QWC = qwc;
    for (;;) {
        madr = D8_MADR;
        tadr = D1_TADR;
        if (madr == tadr) {
            break;
        }
        if ((s32)(((tadr - madr) & D_RBSR) - (u32)(qwc << 4)) > 0) {
            break;
        }
    }
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, D8_CHCR | 0x100);
}
