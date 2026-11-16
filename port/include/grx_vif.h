/* The port's VIF0 and VIF1 (port/src/vif.c): the command interpreters the
   VIF DMA channels feed (grx_dma.h).

   The input is a word stream; a command and its data may be split across
   any number of writes (DMA tags, MFIFO sends), as on the hardware.

   VIF1: UNPACK (every format, with STCYCL skipping/filling, STMASK, STROW,
   STCOL and STMOD offset/difference) into VU1 data memory and MPG into VU1
   micro memory, both grx_vu1.h's arrays; DIRECT/DIRECTHL to
   grx_gs_gif(..., GRX_GIF_PATH2); BASE, OFFSET, ITOP and the double
   buffer (TOPS/TOP/ITOP, read by the microprogram through grx_vif1_top()
   and grx_vif1_itop()); MSKPATH3, MARK (written to VIF1_MARK in the
   mapped page), NOP and the FLUSHes.

   MSCAL/MSCALF/MSCNT start the microprogram with grx_vu1_run() (grx_vu1.h),
   but not at once: a started program runs when the VIF stream next waits
   for the VU (FLUSHE, FLUSH, FLUSHA, MSCAL, MSCALF, MSCNT, MPG) or when the
   DMA transfer feeding the VIF ends (grx_vif_sync). On the hardware the
   program runs while the VIF goes on unpacking, and the game relies on it:
   a phase reads data unpacked after the MSCNT that started it
   (port/docs/vu.md, "Phases overlap the uploads"). MSCAL passes its
   immediate, the instruction (pair) address as TPC holds it: MSCAL n starts
   at micro memory byte 8n. MSCNT passes GRX_VU1_CONTINUE. TOP and ITOP are
   latched when the program is started, so grx_vif1_top() is right when it
   runs later.

   VIF0 does the same into VU0 memory (the mapped window at 0x11000000:
   micro at +0x0000, data at +0x4000, 4 KB each); its MSCALs are logged and
   ignored (the port runs the VU0 kernels as C called by the CPU code). */
#ifndef GRX_VIF_H
#define GRX_VIF_H

#include <stdint.h>

/* VIFn_FBRST.RST: the decode state, the double buffer and a started
   microprogram are dropped (the registers that the SDK resets with VIF
   codes afterwards are kept) */
void grx_vif_reset(int vif);

/* n words of VIF codes and their data, from the DMA channel */
void grx_vif_write(int vif, const uint32_t *words, uint32_t n);

/* The transfer feeding the VIF ended (or stalled): run a started
   microprogram (VIF1) */
void grx_vif_sync(int vif);

/* Whether VIF1 masks PATH3 (MSKPATH3) */
int grx_vif1_path3_masked(void);

/* 1 if the VIF is in the middle of a command (waiting for its data) */
int grx_vif_busy(int vif);

/* Diagnostics: 1 to log every VIF code to stderr */
void grx_vif_set_trace(int on);

#endif
