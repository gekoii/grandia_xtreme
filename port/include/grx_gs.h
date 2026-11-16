/* The port's GS (Graphics Synthesizer): what the DMA layer feeds it and
   what the frame loop reads from it. port/src/gs/ implements it; the
   GS's privileged registers (PMODE, DISPFB1/2, DISPLAY1/2, BGCOLOR, CSR,
   IMR at 0x12000000) are the memory grx_hwmem.h maps there, where the game
   writes them directly. */
#ifndef GRX_GS_H
#define GRX_GS_H

#include <stdint.h>

/* GIF paths: 1 from VU1 XGKICK, 2 from VIF1 DIRECT/DIRECTHL, 3 from GIF DMA */
enum { GRX_GIF_PATH1 = 1, GRX_GIF_PATH2 = 2, GRX_GIF_PATH3 = 3 };

void grx_gs_init(void);

/* A reset as sceGsResetGraph / CSR.RESET does: registers to their reset
   values (local memory is kept) */
void grx_gs_reset(void);

/* GIF data: qwc quadwords of GIF tags and their data (PACKED, REGLIST,
   IMAGE), as they would arrive over the given path. Processed at once. */
void grx_gs_gif(const void *data, uint32_t qwc, int path);

/* GIF_CTRL.RST (sceGsResetPath): drop any packet cut off part-way, on
   every path, so the next qword on each is a GIF tag */
void grx_gs_gif_reset(void);

/* Local-to-host transfers (BITBLTBUF/TRXPOS/TRXREG/TRXDIR = 1, as
   sceGsExecStoreImage starts): copy what the GS would send back */
void grx_gs_read_image(void *dst, uint32_t qwc);

/* Once per field, from the frame loop: flips CSR.FIELD, sets CSR.VSINT
   (and NFIELD), as the GS does at vblank */
void grx_gs_vsync(void);

/* The picture the GS displays now (PMODE, DISPFB1/2, DISPLAY1/2,
   BGCOLOR, from the mapped privileged registers): 32-bit pixels, 0xAARRGGBB,
   pitch in pixels. The pointer stays valid until the next call. Returns 0
   if the display circuits are off. */
int grx_gs_display(const uint32_t **pixels, int *width, int *height, int *pitch);

/* The GPU renderer (port/src/gs_gl/, port/docs/gpu.md), after grx_gs_init
   and before grx_frame_start, with a window: draws in OpenGL at an
   internal scale (GRX_GS_SCALE, else the smallest whole multiple of 448
   lines that fills the window) and presents through grx_video_gl_submit.
   Returns 0 when it is the GS's renderer from now on, -1 (logged) when it
   couldn't start: the software GS stays. */
int grx_gs_gl_init(void);

#endif
