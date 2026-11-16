#ifndef GRX_SPU2_H
#define GRX_SPU2_H

/*
 * The PS2's sound processor (SPU2), in software: 2 cores of 24 voices,
 * 2 MB of sound memory, PS-ADPCM with the loop flags, the pitch counter
 * with 4-tap Gaussian interpolation, ADSR envelopes, volume sweeps, noise,
 * pitch modulation and the cores' mixing paths (core 0's output goes into
 * core 1, core 1's output is the DAC). Reverb is not done yet: the effect
 * registers are kept, the wet path is silent (port/docs/sound.md).
 *
 * Independent of the game and of GS_IOP's commands (port/src/snd/ sits on
 * top of it). Registers are reached with Sony libsd's encodings
 * (sceSdSetParam & co., the GRX_SD_* values below), so the sound driver can
 * be written the way GS_IOP.IRX calls LIBSD.IRX. Addresses are bytes, as
 * libsd takes them.
 *
 * Not thread-safe: the caller serialises (port/src/audio.c holds the SDL
 * audio stream's lock around everything). Plain C, no OS calls.
 */

#include <stdint.h>

#define GRX_SPU2_RATE      48000
#define GRX_SPU2_RAM_BYTES 0x200000u
#define GRX_SPU2_CORES     2
#define GRX_SPU2_VOICES    24

/* ---- libsd register encodings (entry = what | core | voice << 1) ---- */
#define GRX_SD_CORE(c)    (c)
#define GRX_SD_VOICE(v)   ((v) << 1)
/* voice parameters (16 bits) */
#define GRX_SD_VP_VOLL    0x0000
#define GRX_SD_VP_VOLR    0x0100
#define GRX_SD_VP_PITCH   0x0200
#define GRX_SD_VP_ADSR1   0x0300
#define GRX_SD_VP_ADSR2   0x0400
#define GRX_SD_VP_ENVX    0x0500   /* read only */
#define GRX_SD_VP_VOLXL   0x0600   /* read only: current volume */
#define GRX_SD_VP_VOLXR   0x0700
/* core parameters (16 bits) */
#define GRX_SD_P_MMIX     0x0800
#define GRX_SD_P_MVOLL    0x0980
#define GRX_SD_P_MVOLR    0x0A80
#define GRX_SD_P_EVOLL    0x0B80   /* effect return */
#define GRX_SD_P_EVOLR    0x0C80
#define GRX_SD_P_AVOLL    0x0D80   /* external input (core 1: core 0's output) */
#define GRX_SD_P_AVOLR    0x0E80
#define GRX_SD_P_BVOLL    0x0F80   /* sound data input (AutoDMA) */
#define GRX_SD_P_BVOLR    0x1080
#define GRX_SD_P_MVOLXL   0x1180   /* read only */
#define GRX_SD_P_MVOLXR   0x1280
/* switches (24 bits, one per voice) */
#define GRX_SD_S_PMON     0x1300
#define GRX_SD_S_NON      0x1400
#define GRX_SD_S_KON      0x1500
#define GRX_SD_S_KOFF     0x1600
#define GRX_SD_S_ENDX     0x1700
#define GRX_SD_S_VMIXL    0x1800
#define GRX_SD_S_VMIXEL   0x1900
#define GRX_SD_S_VMIXR    0x1A00
#define GRX_SD_S_VMIXER   0x1B00
/* core addresses */
#define GRX_SD_A_ESA      0x1C00   /* effect work area start */
#define GRX_SD_A_EEA      0x1D00   /* effect work area end */
#define GRX_SD_A_TSA      0x1E00
#define GRX_SD_A_IRQA     0x1F00
/* voice addresses */
#define GRX_SD_VA_SSA     0x2040   /* start */
#define GRX_SD_VA_LSAX    0x2140   /* loop start */
#define GRX_SD_VA_NAX     0x2240   /* current (read) */
/* core attributes (sceSdSetCoreAttr) */
#define GRX_SD_C_EFFECT_ENABLE 0x02
#define GRX_SD_C_IRQ_ENABLE    0x04
#define GRX_SD_C_MUTE_ENABLE   0x06
#define GRX_SD_C_NOISE_CLK     0x08
#define GRX_SD_C_SPDIF_MODE    0x0A

/* MMIX bits (which sources a core sends to its dry output and to its
   effect input): Snd = the core's voices, Inp = sound data input,
   Ext = external input (core 0's output, for core 1) */
#define GRX_SPU2_MMIX_EXT_WET_R 0x001
#define GRX_SPU2_MMIX_EXT_WET_L 0x002
#define GRX_SPU2_MMIX_EXT_DRY_R 0x004
#define GRX_SPU2_MMIX_EXT_DRY_L 0x008
#define GRX_SPU2_MMIX_INP_WET_R 0x010
#define GRX_SPU2_MMIX_INP_WET_L 0x020
#define GRX_SPU2_MMIX_INP_DRY_R 0x040
#define GRX_SPU2_MMIX_INP_DRY_L 0x080
#define GRX_SPU2_MMIX_SND_WET_R 0x100
#define GRX_SPU2_MMIX_SND_WET_L 0x200
#define GRX_SPU2_MMIX_SND_DRY_R 0x400
#define GRX_SPU2_MMIX_SND_DRY_L 0x800

/* ADPCM block flags (byte 1 of a block) */
#define GRX_SPU2_FLAG_END        0x01  /* jump to the loop start after this block */
#define GRX_SPU2_FLAG_REPEAT     0x02  /* with END: keep playing (else the voice stops) */
#define GRX_SPU2_FLAG_LOOP_START 0x04  /* this block becomes the loop start */

/* Reverb settings as sceSdSetEffectAttr takes them (kept, not used yet) */
typedef struct GrxSpu2EffectAttr {
    int core;
    int mode;           /* 0 off .. 9 pipe; | 0x100 clears the work area */
    int16_t depth_l, depth_r;
    int delay, feedback;
} GrxSpu2EffectAttr;

typedef struct GrxSpu2 GrxSpu2;

GrxSpu2 *grx_spu2_create(void);       /* reset state, RAM zeroed */
void grx_spu2_destroy(GrxSpu2 *s);
void grx_spu2_reset(GrxSpu2 *s);       /* registers and voices; RAM kept */

/* Sound memory */
uint8_t *grx_spu2_ram(GrxSpu2 *s);
void grx_spu2_ram_write(GrxSpu2 *s, uint32_t addr, const void *src, uint32_t n);
void grx_spu2_ram_read(GrxSpu2 *s, uint32_t addr, void *dst, uint32_t n);

/* Registers (libsd's calls) */
void grx_spu2_set_param(GrxSpu2 *s, uint32_t entry, uint16_t value);
uint16_t grx_spu2_get_param(GrxSpu2 *s, uint32_t entry);
void grx_spu2_set_switch(GrxSpu2 *s, uint32_t entry, uint32_t value);
uint32_t grx_spu2_get_switch(GrxSpu2 *s, uint32_t entry);
void grx_spu2_set_addr(GrxSpu2 *s, uint32_t entry, uint32_t addr);
uint32_t grx_spu2_get_addr(GrxSpu2 *s, uint32_t entry);
void grx_spu2_set_core_attr(GrxSpu2 *s, uint32_t entry, int value);
int grx_spu2_get_core_attr(GrxSpu2 *s, uint32_t entry);
void grx_spu2_set_effect_attr(GrxSpu2 *s, const GrxSpu2EffectAttr *a);

/* Sound data input of a core (what AutoDMA feeds: stereo s16 frames,
   mixed through BVOL and the Inp gates). Called once per output frame
   while set; NULL to clear. Returns 0 when it has nothing (silence). */
typedef int (*GrxSpu2InputFn)(void *ctx, int core, int16_t lr[2]);
void grx_spu2_set_input(GrxSpu2 *s, int core, GrxSpu2InputFn fn, void *ctx);

/* Runs the chip for `frames` 48 kHz output frames, interleaved L/R s16 */
void grx_spu2_mix(GrxSpu2 *s, int16_t *out, int frames);

/* ---- Building blocks, exposed for the tests ---- */

/* Decodes one 16-byte block. hist[0] is the last sample decoded, hist[1]
   the one before; both are updated. Returns the block's flags byte. */
int grx_spu2_adpcm_block(const uint8_t blk[16], int32_t hist[2], int16_t out[28]);

/* The 512-entry interpolation table, and one interpolated sample:
   s[0] the oldest of the last four samples, s[3] the newest; phase 0..255
   (bits 4-11 of the pitch counter). */
const int16_t *grx_spu2_gauss_table(void);
int grx_spu2_interpolate(const int16_t s[4], int phase);

/* The envelope engine shared by ADSR and volume sweeps: one tick per
   output sample. rate is the 7-bit form (shift = rate >> 2, step from
   rate & 3; decay and release rates are given as r << 2). */
typedef struct GrxSpu2Env {
    int32_t level;      /* 0..0x7FFF */
    uint32_t counter;
    uint8_t rate, exponential, decreasing;
} GrxSpu2Env;
void grx_spu2_env_start(GrxSpu2Env *e, int rate, int exponential, int decreasing);
void grx_spu2_env_tick(GrxSpu2Env *e);

/* ADSR phases (grx_spu2_voice_phase) */
enum { GRX_SPU2_OFF, GRX_SPU2_ATTACK, GRX_SPU2_DECAY, GRX_SPU2_SUSTAIN, GRX_SPU2_RELEASE };
int grx_spu2_voice_phase(GrxSpu2 *s, int core, int voice);

#endif
