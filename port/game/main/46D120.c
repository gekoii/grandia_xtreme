/* Port-only C for sceGsResetPath (0x0046D120), which the decompilation still has in assembly. */
/* Sony PS2 SDK library code (libgraph): see README.md, "Third-party library code". */
#include "common.h"
#include "grx_vif.h"
#include "grx_gs.h"

#define VIF1_FBRST (*(volatile u32 *)0x10003C10)
#define VIF1_ERR (*(volatile u32 *)0x10003C20)
#define GIF_CTRL (*(volatile u32 *)0x10003000)

void sceGsResetPath(void);

/* _sceGsResetPathVifCodes (0x0049DD90), still asm-only data: STCYCL 4,4;
   STMASK 0; NOP; STMOD 0; MSKPATH3 off; BASE 0; OFFSET 0; ITOP 0 */
static const u32 resetPathVifCodes[8] __attribute__((aligned(16))) = {
    0x01000404, 0x20000000, 0x00000000, 0x05000000, 0x06000000, 0x03000000, 0x02000000, 0x04000000,
};

/* Resets the paths to the GS: VIF1 (FBRST.RST), its error mask (ERR.ME0,
   so a DMA tag mismatch doesn't stop it), VU1 (VU0's FBRST via vi28, bit
   9: RST1), VIF1's registers with the two quadwords of codes above written
   to VIF1_FIFO, and the GIF (GIF_CTRL.RST).

   In the port: VIF1's reset and the codes go to port/src/vif.c. VU1 has
   no state of its own to reset beyond the started program, which the VIF
   reset drops. The GIF reset drops a partial packet on any path
   (grx_gs_gif_reset). The register stores are kept for the record. */
void sceGsResetPath(void) {
    VIF1_FBRST = 1;
    grx_vif_reset(1);
    VIF1_ERR = 2;
    grx_vif_write(1, resetPathVifCodes, 8);
    GIF_CTRL = 1;
    grx_gs_gif_reset();
}
