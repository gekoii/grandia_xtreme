/* The platform layer's parts in a save state (port/src/state.c): each
   passes the PS2 state it stands in for through grx_state_io, the same
   way when saving and loading, and leaves out what belongs to this run
   (host pointers, handles, threads, settings). Some refuse to be saved at
   a given moment (*_blocker: why, or NULL). */
#ifndef GRX_STATE_PARTS_H
#define GRX_STATE_PARTS_H

#include "grx_state.h"

void grx_kernel_state_io(GrxState *s);          /* kernel.c: threads, semaphores, handlers, alarms, clock */
const char *grx_kernel_state_blocker(void);
void *grx_kernel_main_wait_site(void);          /* the game's call the main thread waits in */
void grx_cdvd_state_io(GrxState *s);            /* cdvd.c: the IOP block, GS_IOP's status and streams */
void grx_sif_state_io(GrxState *s);             /* sif.c: RPC servers, modules, SIF registers */
void grx_dma_state_io(GrxState *s);             /* dma.c: nothing, between fields */
const char *grx_dma_state_blocker(void);
void grx_ee_state_io(GrxState *s);              /* ee.c, ee_misc.c: VU0 registers, the CRT setting */
void grx_vif_state_io(GrxState *s);             /* vif.c, vu1/vu1.c: VIFs, VU1 memory and registers */
void grx_vu1_state_io(GrxState *s);             /* vu1/vu1.c, from vif.c's */
void grx_gs_state_io(GrxState *s);              /* gs/gs_record.c: GS registers and local memory */
const char *grx_gs_state_blocker(void);
void grx_gs_state_loaded(void);
void grx_snd_state_io(GrxState *s);             /* snd/snd.c, spu2/spu2.c: the driver, SPU2 registers and RAM */
struct GrxSpu2;
void grx_spu2_state_io(struct GrxSpu2 *sp, GrxState *s); /* spu2/spu2.c, from snd.c's */
void grx_mips_state_io(GrxState *s);            /* mips/cpu.c, mips/thunks.c: load areas, thunks */
const char *grx_mips_state_blocker(void);
void grx_mips_state_loaded(void);
void grx_pad_state_io(GrxState *s);             /* pad.c: libpad's ports */
void grx_mc_state_io(GrxState *s);              /* mc.c: libmc's pending call, the cards' state */
const char *grx_mc_state_blocker(void);
void grx_movie_state_io(GrxState *s);           /* movie/iop_movie.c: the IOP side's streams */
const char *grx_movie_state_blocker(void);
void grx_video_state_loaded(void);              /* video.c: the field clock starts again from now */

/* programs.c: the disc programs' memory, set up now if it isn't */
void grx_programs_init(void);

#define GRX_STATE_PARTS                                              \
    {"kernel", grx_kernel_state_io, NULL},                           \
    {"cdvd", grx_cdvd_state_io, NULL},                               \
    {"sif", grx_sif_state_io, NULL},                                 \
    {"dma", grx_dma_state_io, NULL},                                 \
    {"ee", grx_ee_state_io, NULL},                                   \
    {"vif", grx_vif_state_io, NULL},                                 \
    {"gs", grx_gs_state_io, grx_gs_state_loaded},                    \
    {"snd", grx_snd_state_io, NULL},                                 \
    {"mips", grx_mips_state_io, grx_mips_state_loaded},              \
    {"pad", grx_pad_state_io, NULL},                                 \
    {"mc", grx_mc_state_io, NULL},                                   \
    {"movie", grx_movie_state_io, NULL},                             \
    {"video", NULL, grx_video_state_loaded},

#define GRX_STATE_BLOCKERS                                           \
    grx_kernel_state_blocker, grx_dma_state_blocker, grx_gs_state_blocker, \
    grx_mips_state_blocker, grx_mc_state_blocker, grx_movie_state_blocker,

#endif
