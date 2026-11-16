/* The port's DMA controller (port/src/dma.c, port/docs/hardware.md (b)).

   DMA is synchronous: when the game starts a channel (a CHCR store with
   STR set), the whole transfer runs before the call returns. Then, as the
   hardware does at the end of a transfer, the channel's registers in the
   mapped page at 0x10000000 are written back (CHCR.STR cleared, QWC 0,
   MADR/TADR/SADR advanced, ASP and TAG updated), D_STAT's bit for the
   channel is set, and the kernel's DMAC handlers for it run
   (grx_kernel_raise_dmac). So the game's polls on CHCR, QWC and MADR see a
   finished transfer without any change.

   The registers themselves stay plain memory the game writes: only the
   store that starts a transfer has to come here. The SDK's libdma
   (sceDmaSend, ...) and the game's own CHCR stores do that through
   grx_dma_kick()/grx_dma_write_chcr() (port/tools/fixups/dma.py, the
   port C in port/game/main/).

   Addresses in MADR, TADR and tags are EE addresses: bit 31 (tags: bit 63)
   or 0x70000000-0x70003FFF is the scratchpad; anything else is the port's
   own address of the memory (below 0x08000000, port/docs/hardware.md (a) 3),
   masked with 0x0FFFFFF0 as the DMAC does. SADR is a scratchpad offset.

   Channels: VIF0 (0) and VIF1 (1) feed the VIF interpreters (grx_vif.h),
   GIF (2) goes to grx_gs_gif(..., GRX_GIF_PATH3), fromSPR/toSPR (8/9) copy,
   in normal, chain and interleave mode, and MFIFO (D_CTRL.MFD) sends
   fromSPR into the ring and drains it through VIF1 or GIF at once. IPU
   (3/4) is reported and dropped (movies are phase 5). SIF (5/6/7) belongs
   to port/src/sif.c: it can take its channels over with
   grx_dma_set_channel_hook(). */
#ifndef GRX_DMA_H
#define GRX_DMA_H

#include <stdint.h>

/* Channels (the DMAC's numbering, as grx_kernel.h's GRX_DMAC_*) */
enum {
    GRX_DMA_VIF0 = 0,
    GRX_DMA_VIF1 = 1,
    GRX_DMA_GIF = 2,
    GRX_DMA_FROM_IPU = 3,
    GRX_DMA_TO_IPU = 4,
    GRX_DMA_SIF0 = 5,
    GRX_DMA_SIF1 = 6,
    GRX_DMA_SIF2 = 7,
    GRX_DMA_FROM_SPR = 8,
    GRX_DMA_TO_SPR = 9,
    GRX_DMA_NCHANNELS = 10
};

/* Register addresses: a channel's block, and the offsets in it */
#define GRX_DMA_D0 0x10008000u
#define GRX_DMA_D1 0x10009000u
#define GRX_DMA_D2 0x1000A000u
#define GRX_DMA_D3 0x1000B000u
#define GRX_DMA_D4 0x1000B400u
#define GRX_DMA_D5 0x1000C000u
#define GRX_DMA_D6 0x1000C400u
#define GRX_DMA_D7 0x1000C800u
#define GRX_DMA_D8 0x1000D000u
#define GRX_DMA_D9 0x1000D400u
#define GRX_DMA_CHCR 0x00u
#define GRX_DMA_MADR 0x10u
#define GRX_DMA_QWC 0x20u
#define GRX_DMA_TADR 0x30u
#define GRX_DMA_ASR0 0x40u
#define GRX_DMA_ASR1 0x50u
#define GRX_DMA_SADR 0x80u
/* The DMAC's common registers */
#define GRX_D_CTRL 0x1000E000u
#define GRX_D_STAT 0x1000E010u
#define GRX_D_PCR 0x1000E020u
#define GRX_D_SQWC 0x1000E030u
#define GRX_D_RBSR 0x1000E040u
#define GRX_D_RBOR 0x1000E050u

/* CHCR bits */
#define GRX_CHCR_DIR 0x001u /* 1: from memory */
#define GRX_CHCR_MOD 0x00Cu /* 0 normal, 1 chain, 2 interleave */
#define GRX_CHCR_ASP 0x030u /* call depth */
#define GRX_CHCR_TTE 0x040u /* VIF: send the tag's upper 64 bits too */
#define GRX_CHCR_TIE 0x080u /* a tag with IRQ ends the transfer */
#define GRX_CHCR_STR 0x100u

/* The base address of channel ch's registers, or 0 */
uint32_t grx_dma_chan_base(int ch);

/* The channel whose register block holds the address (a sceDmaChan *),
   or -1 */
int grx_dma_channel(uintptr_t reg);

/* The game wrote channel ch's CHCR (through a sceDmaChan pointer:
   grx_dma_kick). If STR is set, the transfer runs now and the registers
   are written back. Nothing happens if STR is clear (a stop). */
void grx_dma_start(int ch);
void grx_dma_kick(volatile void *chan);

/* A CHCR store: writes the value into the mapped register, then
   grx_dma_start(). For the port C that used to store CHCR itself. */
void grx_dma_write_chcr(int ch, uint32_t chcr);

/* Wait for a channel to stop: 0 at once if STR is clear. With synchronous
   DMA STR can stay set only for a channel that is stalled for good (the
   MFIFO drain channel waiting for data, a transfer that failed); that
   is logged, and -1 returned, rather than spinning. */
int grx_dma_wait(int ch);

/* sceDmaReset ran (the channel registers and D_STAT are cleared) */
void grx_dma_reset(void);

/* Channels another part of the platform runs itself (SIF, port/src/sif.c):
   the hook is called instead of the transfer and must do all of it,
   write-back and grx_kernel_raise_dmac included. NULL removes it. */
typedef void (*GrxDmaChannelHook)(int ch);
void grx_dma_set_channel_hook(int ch, GrxDmaChannelHook hook);

/* Diagnostics: 1 to log every transfer and tag to stderr */
void grx_dma_set_trace(int on);

/* A store to the GS's CSR (0x12001000) with the real register's meaning,
   for every game site that writes it (fixups/dma.py) and for the platform:
   SIGNAL, FINISH, HSINT, VSINT and EDWINT (bits 0-4) written as 1 are
   cleared; RESET (bit 9) resets the GS (grx_gs_reset, which also sets
   REV/ID and keeps FIELD); FLUSH (bit 8) has nothing to flush. NFIELD,
   FIELD, FIFO, REV and ID are read-only and kept. The mapped CSR is plain
   memory, where those stores would set the bits instead.
   grx_gs_vsync() is what sets VSINT/NFIELD and flips FIELD. */
#define GRX_GS_CSR 0x12001000u
void grx_gs_csr_write(uint64_t value);

/* The SIO's transmit FIFO (_sceSioPutRawChar, the SDK's debug output):
   one character to stderr */
void grx_sio_putc(int c);

#endif
