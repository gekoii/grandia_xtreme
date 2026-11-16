/*
 * SPU2 in software (port/include/grx_spu2.h, port/docs/sound.md).
 *
 * Behaviour follows the documented PS1 SPU, which the SPU2's voices carry
 * over unchanged (ADPCM, ADSR, sweeps, noise, the pitch counter), and the
 * SPU2's own two-core mixing. Written from the hardware descriptions, not
 * from another emulator's code.
 *
 * What is not done: reverb (the effect registers are stored, the wet path
 * is silent), IRQs, the voices' and cores' output capture, SPDIF. The
 * interpolation table is generated (a Gaussian with the hardware's shape
 * and phase layout), not the hardware's exact values.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "grx_spu2.h"
#include "grx_state_parts.h"

typedef struct {
    uint16_t reg;       /* the register as written */
    int16_t cur;        /* current volume */
    GrxSpu2Env env;     /* sweep mode */
} Vol;

typedef struct {
    uint16_t pitch, adsr1, adsr2;
    Vol vl, vr;
    uint32_t ssa, lsa, cur;   /* byte addresses; cur is the block playing */
    GrxSpu2Env env;
    int phase;
    uint32_t counter;         /* pitch counter, 12-bit fraction */
    int16_t blk[28];          /* the decoded samples of cur */
    int pos;                  /* next of them */
    uint8_t flags;            /* cur's flags */
    int32_t hist[2];
    int16_t s[4];             /* last four samples, s[3] newest */
    int32_t out;              /* last output before volume (PMON) */
} Voice;

typedef struct {
    Voice v[GRX_SPU2_VOICES];
    uint32_t pmon, non, vmixl, vmixel, vmixr, vmixer, endx;
    uint16_t mmix;
    Vol mvl, mvr;
    int16_t evoll, evolr, avoll, avolr, bvoll, bvolr;
    uint32_t esa, eea, tsa, irqa;
    int effect_enable, irq_enable, mute, noise_clk, spdif;
    int16_t noise;
    int32_t noise_timer;
    GrxSpu2EffectAttr fx;
    GrxSpu2InputFn in;
    void *in_ctx;
} Core;

struct GrxSpu2 {
    uint8_t *ram;
    Core c[GRX_SPU2_CORES];
};

/* ------------------------------------------------------------------------ */
/* ADPCM                                                                    */

int grx_spu2_adpcm_block(const uint8_t blk[16], int32_t hist[2], int16_t out[28]) {
    static const int f0[5] = { 0, 60, 115, 98, 122 };
    static const int f1[5] = { 0, 0, -52, -55, -60 };
    int shift = blk[0] & 0x0F;
    int filter = (blk[0] >> 4) & 0x0F;
    int i;

    /* shifts 13-15 act as 9; filters past 4 as 4 */
    if (shift > 12) {
        shift = 9;
    }
    if (filter > 4) {
        filter = 4;
    }
    for (i = 0; i < 28; i++) {
        int nib = (blk[2 + (i >> 1)] >> ((i & 1) * 4)) & 0x0F;
        int32_t v = (int32_t)(int16_t)(uint16_t)(nib << 12) >> shift;
        v += (hist[0] * f0[filter] + hist[1] * f1[filter] + 32) >> 6;
        if (v > 32767) {
            v = 32767;
        } else if (v < -32768) {
            v = -32768;
        }
        hist[1] = hist[0];
        hist[0] = v;
        out[i] = (int16_t)v;
    }
    return blk[1];
}

/* ------------------------------------------------------------------------ */
/* Interpolation                                                            */

static int16_t gauss[512];
static int gauss_ready;

/* g[k] weighs a sample (512 - k) / 256 samples away from the point being
   interpolated: g[0x000-0x0FF] the far taps, g[0x100-0x1FF] the near ones,
   used as gauss[0xFF - i], gauss[0x1FF - i], gauss[0x100 + i], gauss[i]
   on the four samples oldest first. A Gaussian with sigma 0.6 sums to
   nearly the same at every phase; scaled so no phase's sum passes 0x7FFF. */
static void gauss_init(void) {
    double g[512];
    double max = 0;
    int k, i;

    for (k = 0; k < 512; k++) {
        double d = (512 - k) / 256.0;
        g[k] = exp(-d * d / (2 * 0.6 * 0.6));
    }
    for (i = 0; i < 256; i++) {
        double sum = g[0xFF - i] + g[0x1FF - i] + g[0x100 + i] + g[i];
        if (sum > max) {
            max = sum;
        }
    }
    for (k = 0; k < 512; k++) {
        gauss[k] = (int16_t)floor(g[k] * 32767.0 / max);
    }
    gauss_ready = 1;
}

const int16_t *grx_spu2_gauss_table(void) {
    if (!gauss_ready) {
        gauss_init();
    }
    return gauss;
}

int grx_spu2_interpolate(const int16_t s[4], int phase) {
    int i = phase & 0xFF;
    int32_t o;

    if (!gauss_ready) {
        gauss_init();
    }
    o = gauss[0x0FF - i] * s[0];
    o += gauss[0x1FF - i] * s[1];
    o += gauss[0x100 + i] * s[2];
    o += gauss[i] * s[3];
    return o >> 15;
}

/* ------------------------------------------------------------------------ */
/* Envelopes                                                                */

void grx_spu2_env_start(GrxSpu2Env *e, int rate, int exponential, int decreasing) {
    e->rate = (uint8_t)(rate & 0x7F);
    e->exponential = (uint8_t)(exponential != 0);
    e->decreasing = (uint8_t)(decreasing != 0);
    e->counter = 0;
}

/* One sample. The level moves by step << (11 - shift) every sample while
   shift < 11, else by step every 2^(shift - 11) samples; step is 7..4
   going up, -8..-5 going down. Exponential: going down the step scales
   with the level, going up it slows to a quarter above 0x6000. Rate 0x7F
   never moves. */
void grx_spu2_env_tick(GrxSpu2Env *e) {
    int rate = e->rate;
    int shift = rate >> 2;
    int32_t step;
    uint32_t inc = 0x8000;
    int32_t level = e->level;

    if (rate == 0x7F) {
        return;
    }
    step = 7 - (rate & 3);
    if (e->decreasing) {
        step = -8 + (rate & 3);
    }
    if (shift < 11) {
        step <<= 11 - shift;
    } else {
        inc >>= shift - 11;
    }
    if (e->exponential) {
        if (e->decreasing) {
            step = (step * level) >> 15;
        } else if (level >= 0x6000) {
            if (shift < 10) {
                step >>= 2;
            } else if (shift >= 11) {
                inc >>= 2;
            } else {
                step >>= 1;
                inc >>= 1;
            }
        }
    }
    e->counter += inc;
    if (!(e->counter & 0x8000)) {
        return;
    }
    e->counter = 0;
    level += step;
    if (level < 0) {
        level = 0;
    } else if (level > 0x7FFF) {
        level = 0x7FFF;
    }
    e->level = level;
}

/* ------------------------------------------------------------------------ */
/* Volumes                                                                  */

/* Bit 15 clear: fixed, bits 14-0 are half the volume (signed). Set: a
   sweep, bit 14 exponential, bit 13 decreasing, bit 12 phase inverted,
   bits 6-0 the rate. */
static void vol_write(Vol *v, uint16_t reg) {
    v->reg = reg;
    if (!(reg & 0x8000)) {
        v->cur = (int16_t)(uint16_t)(reg << 1);
        return;
    }
    v->env.level = v->cur < 0 ? -v->cur : v->cur;
    if (v->env.level > 0x7FFF) {
        v->env.level = 0x7FFF;
    }
    grx_spu2_env_start(&v->env, reg & 0x7F, reg & 0x4000, reg & 0x2000);
}

static void vol_tick(Vol *v) {
    if (!(v->reg & 0x8000)) {
        return;
    }
    grx_spu2_env_tick(&v->env);
    v->cur = (int16_t)((v->reg & 0x1000) ? -v->env.level : v->env.level);
}

/* ------------------------------------------------------------------------ */
/* Voices                                                                   */

static int sustain_level(const Voice *v) {
    int l = ((v->adsr1 & 0x0F) + 1) * 0x800;
    return l > 0x7FFF ? 0x7FFF : l;
}

/* Starts (or re-reads) the current phase's rate from the registers */
static void adsr_phase(Voice *v, int phase, int restart) {
    GrxSpu2Env *e = &v->env;
    uint32_t counter = e->counter;

    v->phase = phase;
    switch (phase) {
    case GRX_SPU2_ATTACK:
        grx_spu2_env_start(e, (v->adsr1 >> 8) & 0x7F, v->adsr1 & 0x8000, 0);
        break;
    case GRX_SPU2_DECAY:
        grx_spu2_env_start(e, ((v->adsr1 >> 4) & 0x0F) << 2, 1, 1);
        break;
    case GRX_SPU2_SUSTAIN:
        grx_spu2_env_start(e, (v->adsr2 >> 6) & 0x7F, v->adsr2 & 0x8000, v->adsr2 & 0x4000);
        break;
    case GRX_SPU2_RELEASE:
        grx_spu2_env_start(e, (v->adsr2 & 0x1F) << 2, v->adsr2 & 0x0020, 1);
        break;
    default:
        e->level = 0;
        break;
    }
    if (!restart) {
        e->counter = counter;
    }
}

static void adsr_tick(Voice *v) {
    switch (v->phase) {
    case GRX_SPU2_ATTACK:
        grx_spu2_env_tick(&v->env);
        if (v->env.level >= 0x7FFF) {
            adsr_phase(v, GRX_SPU2_DECAY, 1);
        }
        break;
    case GRX_SPU2_DECAY:
        grx_spu2_env_tick(&v->env);
        if (v->env.level <= sustain_level(v)) {
            adsr_phase(v, GRX_SPU2_SUSTAIN, 1);
        }
        break;
    case GRX_SPU2_SUSTAIN:
        grx_spu2_env_tick(&v->env);
        break;
    case GRX_SPU2_RELEASE:
        grx_spu2_env_tick(&v->env);
        if (v->env.level <= 0) {
            adsr_phase(v, GRX_SPU2_OFF, 1);
        }
        break;
    default:
        break;
    }
}

static void load_block(GrxSpu2 *s, Voice *v) {
    const uint8_t *b;

    v->cur &= GRX_SPU2_RAM_BYTES - 16;
    b = s->ram + v->cur;
    v->flags = (uint8_t)grx_spu2_adpcm_block(b, v->hist, v->blk);
    if (v->flags & GRX_SPU2_FLAG_LOOP_START) {
        v->lsa = v->cur;
    }
    v->pos = 0;
}

/* The next source sample into s[] */
static void next_sample(GrxSpu2 *s, Core *c, int vi) {
    Voice *v = &c->v[vi];

    if (v->pos >= 28) {
        if (v->flags & GRX_SPU2_FLAG_END) {
            c->endx |= 1u << vi;
            v->cur = v->lsa;
            if (!(v->flags & GRX_SPU2_FLAG_REPEAT)) {
                adsr_phase(v, GRX_SPU2_OFF, 1);
            }
        } else {
            v->cur += 16;
        }
        load_block(s, v);
    }
    v->s[0] = v->s[1];
    v->s[1] = v->s[2];
    v->s[2] = v->s[3];
    v->s[3] = v->blk[v->pos++];
}

static void key_on(GrxSpu2 *s, Core *c, int vi) {
    Voice *v = &c->v[vi];

    v->cur = v->ssa;
    v->hist[0] = v->hist[1] = 0;
    memset(v->s, 0, sizeof(v->s));
    v->counter = 0;
    v->out = 0;
    load_block(s, v);
    v->env.level = 0;
    adsr_phase(v, GRX_SPU2_ATTACK, 1);
    c->endx &= ~(1u << vi);
}

static void key_off(Core *c, int vi) {
    Voice *v = &c->v[vi];

    if (v->phase != GRX_SPU2_OFF) {
        adsr_phase(v, GRX_SPU2_RELEASE, 1);
    }
}

/* One output sample of voice vi, before its volume */
static int32_t voice_sample(GrxSpu2 *s, Core *c, int vi, int32_t prev_out) {
    Voice *v = &c->v[vi];
    int32_t smp, out;
    uint32_t step;
    uint32_t n;

    if (v->phase == GRX_SPU2_OFF) {
        v->out = 0;
        return 0;
    }
    if (c->non & (1u << vi)) {
        smp = c->noise;
    } else {
        smp = grx_spu2_interpolate(v->s, (v->counter >> 4) & 0xFF);
    }
    out = (smp * v->env.level) >> 15;
    adsr_tick(v);

    step = v->pitch;
    if (vi > 0 && (c->pmon & (1u << vi))) {
        int32_t f = prev_out + 0x8000;
        step = (uint32_t)(((int32_t)step * f) >> 15) & 0xFFFF;
    }
    if (step > 0x3FFF) {
        step = 0x3FFF;
    }
    v->counter += step;
    n = v->counter >> 12;
    v->counter &= 0xFFF;
    while (n-- > 0) {
        next_sample(s, c, vi);
    }
    v->out = out;
    return out;
}

/* ------------------------------------------------------------------------ */
/* Cores                                                                    */

static void noise_tick(Core *c) {
    int step = (c->noise_clk & 3) + 4;
    int shift = (c->noise_clk >> 2) & 0x0F;
    uint16_t n = (uint16_t)c->noise;
    int parity = ((n >> 15) ^ (n >> 12) ^ (n >> 11) ^ (n >> 10) ^ 1) & 1;

    c->noise_timer -= step;
    if (c->noise_timer < 0) {
        c->noise = (int16_t)(uint16_t)((n << 1) | parity);
        c->noise_timer += 0x20000 >> shift;
        if (c->noise_timer < 0) {
            c->noise_timer += 0x20000 >> shift;
        }
    }
}

static int32_t clamp16(int32_t v) {
    return v > 32767 ? 32767 : v < -32768 ? -32768 : v;
}

static int32_t apply(int32_t x, int16_t vol) {
    return (x * vol) >> 15;
}

static void core_mix(GrxSpu2 *s, int ci, const int32_t ext[2], int32_t out[2]) {
    Core *c = &s->c[ci];
    int32_t snd_dl = 0, snd_dr = 0, snd_wl = 0, snd_wr = 0;
    int32_t inp[2] = { 0, 0 };
    int32_t ex[2];
    int32_t dl, dr, wl, wr;
    int32_t prev = 0;
    int vi;

    noise_tick(c);
    for (vi = 0; vi < GRX_SPU2_VOICES; vi++) {
        Voice *v = &c->v[vi];
        int32_t o = voice_sample(s, c, vi, prev);
        uint32_t bit = 1u << vi;
        int32_t l = apply(o, v->vl.cur);
        int32_t r = apply(o, v->vr.cur);

        prev = o;
        vol_tick(&v->vl);
        vol_tick(&v->vr);
        if (c->vmixl & bit) {
            snd_dl += l;
        }
        if (c->vmixr & bit) {
            snd_dr += r;
        }
        if (c->vmixel & bit) {
            snd_wl += l;
        }
        if (c->vmixer & bit) {
            snd_wr += r;
        }
    }
    if (c->in != NULL) {
        int16_t lr[2] = { 0, 0 };
        if (c->in(c->in_ctx, ci, lr)) {
            inp[0] = apply(lr[0], c->bvoll);
            inp[1] = apply(lr[1], c->bvolr);
        }
    }
    ex[0] = apply(ext[0], c->avoll);
    ex[1] = apply(ext[1], c->avolr);

    dl = dr = wl = wr = 0;
    if (c->mmix & GRX_SPU2_MMIX_SND_DRY_L) dl += snd_dl;
    if (c->mmix & GRX_SPU2_MMIX_SND_DRY_R) dr += snd_dr;
    if (c->mmix & GRX_SPU2_MMIX_SND_WET_L) wl += snd_wl;
    if (c->mmix & GRX_SPU2_MMIX_SND_WET_R) wr += snd_wr;
    if (c->mmix & GRX_SPU2_MMIX_INP_DRY_L) dl += inp[0];
    if (c->mmix & GRX_SPU2_MMIX_INP_DRY_R) dr += inp[1];
    if (c->mmix & GRX_SPU2_MMIX_INP_WET_L) wl += inp[0];
    if (c->mmix & GRX_SPU2_MMIX_INP_WET_R) wr += inp[1];
    if (c->mmix & GRX_SPU2_MMIX_EXT_DRY_L) dl += ex[0];
    if (c->mmix & GRX_SPU2_MMIX_EXT_DRY_R) dr += ex[1];
    if (c->mmix & GRX_SPU2_MMIX_EXT_WET_L) wl += ex[0];
    if (c->mmix & GRX_SPU2_MMIX_EXT_WET_R) wr += ex[1];
    /* Reverb: not done yet. wl/wr would go through the effect and come
       back scaled by EVOL. */
    (void)wl;
    (void)wr;

    out[0] = apply(clamp16(dl), c->mvl.cur);
    out[1] = apply(clamp16(dr), c->mvr.cur);
    vol_tick(&c->mvl);
    vol_tick(&c->mvr);
    if (c->mute) {
        out[0] = out[1] = 0;
    }
}

void grx_spu2_mix(GrxSpu2 *s, int16_t *out, int frames) {
    static const int32_t none[2] = { 0, 0 };
    int i;

    for (i = 0; i < frames; i++) {
        int32_t o0[2], o1[2];
        core_mix(s, 0, none, o0);
        o0[0] = clamp16(o0[0]);
        o0[1] = clamp16(o0[1]);
        core_mix(s, 1, o0, o1);
        out[2 * i] = (int16_t)clamp16(o1[0]);
        out[2 * i + 1] = (int16_t)clamp16(o1[1]);
    }
}

/* ------------------------------------------------------------------------ */
/* Registers                                                                */

void grx_spu2_set_param(GrxSpu2 *s, uint32_t entry, uint16_t value) {
    Core *c = &s->c[entry & 1];
    Voice *v = &c->v[((entry >> 1) & 0x1F) % GRX_SPU2_VOICES];

    switch (entry & 0xFF00) {
    case GRX_SD_VP_VOLL: vol_write(&v->vl, value); break;
    case GRX_SD_VP_VOLR: vol_write(&v->vr, value); break;
    case GRX_SD_VP_PITCH: v->pitch = value & 0x3FFF; break;
    case GRX_SD_VP_ADSR1:
        v->adsr1 = value;
        if (v->phase == GRX_SPU2_ATTACK || v->phase == GRX_SPU2_DECAY) {
            adsr_phase(v, v->phase, 0);
        }
        break;
    case GRX_SD_VP_ADSR2:
        v->adsr2 = value;
        if (v->phase == GRX_SPU2_SUSTAIN || v->phase == GRX_SPU2_RELEASE) {
            adsr_phase(v, v->phase, 0);
        }
        break;
    case GRX_SD_VP_ENVX: v->env.level = value & 0x7FFF; break;
    case GRX_SD_P_MMIX & 0xFF00: c->mmix = value; break;
    case GRX_SD_P_MVOLL & 0xFF00: vol_write(&c->mvl, value); break;
    case GRX_SD_P_MVOLR & 0xFF00: vol_write(&c->mvr, value); break;
    case GRX_SD_P_EVOLL & 0xFF00: c->evoll = (int16_t)value; break;
    case GRX_SD_P_EVOLR & 0xFF00: c->evolr = (int16_t)value; break;
    case GRX_SD_P_AVOLL & 0xFF00: c->avoll = (int16_t)value; break;
    case GRX_SD_P_AVOLR & 0xFF00: c->avolr = (int16_t)value; break;
    case GRX_SD_P_BVOLL & 0xFF00: c->bvoll = (int16_t)value; break;
    case GRX_SD_P_BVOLR & 0xFF00: c->bvolr = (int16_t)value; break;
    default: break;
    }
}

uint16_t grx_spu2_get_param(GrxSpu2 *s, uint32_t entry) {
    Core *c = &s->c[entry & 1];
    Voice *v = &c->v[((entry >> 1) & 0x1F) % GRX_SPU2_VOICES];

    switch (entry & 0xFF00) {
    case GRX_SD_VP_VOLL: return v->vl.reg;
    case GRX_SD_VP_VOLR: return v->vr.reg;
    case GRX_SD_VP_PITCH: return v->pitch;
    case GRX_SD_VP_ADSR1: return v->adsr1;
    case GRX_SD_VP_ADSR2: return v->adsr2;
    case GRX_SD_VP_ENVX: return (uint16_t)v->env.level;
    case GRX_SD_VP_VOLXL: return (uint16_t)v->vl.cur;
    case GRX_SD_VP_VOLXR: return (uint16_t)v->vr.cur;
    case GRX_SD_P_MMIX & 0xFF00: return c->mmix;
    case GRX_SD_P_MVOLL & 0xFF00: return c->mvl.reg;
    case GRX_SD_P_MVOLR & 0xFF00: return c->mvr.reg;
    case GRX_SD_P_EVOLL & 0xFF00: return (uint16_t)c->evoll;
    case GRX_SD_P_EVOLR & 0xFF00: return (uint16_t)c->evolr;
    case GRX_SD_P_AVOLL & 0xFF00: return (uint16_t)c->avoll;
    case GRX_SD_P_AVOLR & 0xFF00: return (uint16_t)c->avolr;
    case GRX_SD_P_BVOLL & 0xFF00: return (uint16_t)c->bvoll;
    case GRX_SD_P_BVOLR & 0xFF00: return (uint16_t)c->bvolr;
    case GRX_SD_P_MVOLXL & 0xFF00: return (uint16_t)c->mvl.cur;
    case GRX_SD_P_MVOLXR & 0xFF00: return (uint16_t)c->mvr.cur;
    default: return 0;
    }
}

void grx_spu2_set_switch(GrxSpu2 *s, uint32_t entry, uint32_t value) {
    Core *c = &s->c[entry & 1];
    int i;

    value &= 0xFFFFFF;
    switch (entry & 0xFF00) {
    case GRX_SD_S_PMON: c->pmon = value & ~1u; break;
    case GRX_SD_S_NON: c->non = value; break;
    case GRX_SD_S_KON:
        for (i = 0; i < GRX_SPU2_VOICES; i++) {
            if (value & (1u << i)) {
                key_on(s, c, i);
            }
        }
        break;
    case GRX_SD_S_KOFF:
        for (i = 0; i < GRX_SPU2_VOICES; i++) {
            if (value & (1u << i)) {
                key_off(c, i);
            }
        }
        break;
    case GRX_SD_S_ENDX: c->endx = value; break;
    case GRX_SD_S_VMIXL: c->vmixl = value; break;
    case GRX_SD_S_VMIXEL: c->vmixel = value; break;
    case GRX_SD_S_VMIXR: c->vmixr = value; break;
    case GRX_SD_S_VMIXER: c->vmixer = value; break;
    default: break;
    }
}

uint32_t grx_spu2_get_switch(GrxSpu2 *s, uint32_t entry) {
    Core *c = &s->c[entry & 1];

    switch (entry & 0xFF00) {
    case GRX_SD_S_PMON: return c->pmon;
    case GRX_SD_S_NON: return c->non;
    case GRX_SD_S_ENDX: return c->endx;
    case GRX_SD_S_VMIXL: return c->vmixl;
    case GRX_SD_S_VMIXEL: return c->vmixel;
    case GRX_SD_S_VMIXR: return c->vmixr;
    case GRX_SD_S_VMIXER: return c->vmixer;
    default: return 0;
    }
}

void grx_spu2_set_addr(GrxSpu2 *s, uint32_t entry, uint32_t addr) {
    Core *c = &s->c[entry & 1];
    Voice *v = &c->v[((entry >> 1) & 0x1F) % GRX_SPU2_VOICES];

    addr &= (GRX_SPU2_RAM_BYTES - 1) & ~1u;
    switch (entry & 0xFFC0) {
    case GRX_SD_VA_SSA: v->ssa = addr; return;
    case GRX_SD_VA_LSAX: v->lsa = addr & ~15u; return;
    case GRX_SD_VA_NAX: return;
    default: break;
    }
    switch (entry & 0xFF00) {
    case GRX_SD_A_ESA: c->esa = addr; break;
    case GRX_SD_A_EEA: c->eea = addr | 1; break;
    case GRX_SD_A_TSA: c->tsa = addr; break;
    case GRX_SD_A_IRQA: c->irqa = addr; break;
    default: break;
    }
}

uint32_t grx_spu2_get_addr(GrxSpu2 *s, uint32_t entry) {
    Core *c = &s->c[entry & 1];
    Voice *v = &c->v[((entry >> 1) & 0x1F) % GRX_SPU2_VOICES];

    switch (entry & 0xFFC0) {
    case GRX_SD_VA_SSA: return v->ssa;
    case GRX_SD_VA_LSAX: return v->lsa;
    /* the address of the next sample: the block playing, and the byte the
       nibble is in */
    case GRX_SD_VA_NAX: return v->cur + 2 + (uint32_t)(v->pos >> 1);
    default: break;
    }
    switch (entry & 0xFF00) {
    case GRX_SD_A_ESA: return c->esa;
    case GRX_SD_A_EEA: return c->eea;
    case GRX_SD_A_TSA: return c->tsa;
    case GRX_SD_A_IRQA: return c->irqa;
    default: return 0;
    }
}

void grx_spu2_set_core_attr(GrxSpu2 *s, uint32_t entry, int value) {
    Core *c = &s->c[entry & 1];

    switch (entry & ~1u) {
    case GRX_SD_C_EFFECT_ENABLE: c->effect_enable = value != 0; break;
    case GRX_SD_C_IRQ_ENABLE: c->irq_enable = value != 0; break;
    case GRX_SD_C_MUTE_ENABLE: c->mute = value != 0; break;
    case GRX_SD_C_NOISE_CLK: c->noise_clk = value & 0x3F; break;
    case GRX_SD_C_SPDIF_MODE: c->spdif = value; break;
    default: break;
    }
}

int grx_spu2_get_core_attr(GrxSpu2 *s, uint32_t entry) {
    Core *c = &s->c[entry & 1];

    switch (entry & ~1u) {
    case GRX_SD_C_EFFECT_ENABLE: return c->effect_enable;
    case GRX_SD_C_IRQ_ENABLE: return c->irq_enable;
    case GRX_SD_C_MUTE_ENABLE: return c->mute;
    case GRX_SD_C_NOISE_CLK: return c->noise_clk;
    case GRX_SD_C_SPDIF_MODE: return c->spdif;
    default: return 0;
    }
}

void grx_spu2_set_effect_attr(GrxSpu2 *s, const GrxSpu2EffectAttr *a) {
    s->c[a->core & 1].fx = *a;
}

void grx_spu2_set_input(GrxSpu2 *s, int core, GrxSpu2InputFn fn, void *ctx) {
    s->c[core & 1].in = fn;
    s->c[core & 1].in_ctx = ctx;
}

int grx_spu2_voice_phase(GrxSpu2 *s, int core, int voice) {
    return s->c[core & 1].v[voice % GRX_SPU2_VOICES].phase;
}

/* ------------------------------------------------------------------------ */
/* Memory, setup                                                            */

uint8_t *grx_spu2_ram(GrxSpu2 *s) {
    return s->ram;
}

void grx_spu2_ram_write(GrxSpu2 *s, uint32_t addr, const void *src, uint32_t n) {
    const uint8_t *p = src;

    while (n > 0) {
        uint32_t a = addr & (GRX_SPU2_RAM_BYTES - 1);
        uint32_t run = GRX_SPU2_RAM_BYTES - a;
        if (run > n) {
            run = n;
        }
        memcpy(s->ram + a, p, run);
        p += run;
        addr += run;
        n -= run;
    }
}

void grx_spu2_ram_read(GrxSpu2 *s, uint32_t addr, void *dst, uint32_t n) {
    uint8_t *p = dst;

    while (n > 0) {
        uint32_t a = addr & (GRX_SPU2_RAM_BYTES - 1);
        uint32_t run = GRX_SPU2_RAM_BYTES - a;
        if (run > n) {
            run = n;
        }
        memcpy(p, s->ram + a, run);
        p += run;
        addr += run;
        n -= run;
    }
}

void grx_spu2_reset(GrxSpu2 *s) {
    int ci;

    memset(s->c, 0, sizeof(s->c));
    for (ci = 0; ci < GRX_SPU2_CORES; ci++) {
        Core *c = &s->c[ci];
        /* what the chip comes up with after libsd's sceSdInit: inputs at
           full volume, masters and effect return closed */
        c->avoll = c->avolr = 0x7FFF;
        c->bvoll = c->bvolr = 0x7FFF;
        c->noise = 1;
    }
    if (!gauss_ready) {
        gauss_init();
    }
}

GrxSpu2 *grx_spu2_create(void) {
    GrxSpu2 *s = calloc(1, sizeof(*s));

    if (s == NULL) {
        return NULL;
    }
    s->ram = calloc(1, GRX_SPU2_RAM_BYTES);
    if (s->ram == NULL) {
        free(s);
        return NULL;
    }
    grx_spu2_reset(s);
    return s;
}

void grx_spu2_destroy(GrxSpu2 *s) {
    if (s != NULL) {
        free(s->ram);
        free(s);
    }
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h, through snd.c's part, under the sound
   lock): sound RAM and both cores; the input hooks are this run's */
void grx_spu2_state_io(GrxSpu2 *sp, GrxState *st) {
    int i;
    grx_state_io(st, sp->ram, GRX_SPU2_RAM_BYTES);
    for (i = 0; i < GRX_SPU2_CORES; i++) {
        GrxSpu2InputFn in = sp->c[i].in;
        void *ctx = sp->c[i].in_ctx;
        GRX_STATE_VAR(st, sp->c[i]);
        sp->c[i].in = in;
        sp->c[i].in_ctx = ctx;
    }
}
