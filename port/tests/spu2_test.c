/*
 * Tests of the software SPU2 (port/src/spu2/) and the sound driver on top
 * of it (port/src/snd/). port/docs/sound.md has the build line.
 *
 * Unit tests on vectors built here: PS-ADPCM decoding (filters, shifts,
 * rounding, clamping, flags), loop flags and ENDX, the envelope engine and
 * ADSR phases, volume sweeps, the pitch counter and interpolation (a
 * sine encoded here, played at three pitches), noise, the cores' routing.
 *
 * With the game's files extracted to iso/ (or ISO=dir), the driver is run
 * the way the EE drives it, with cdvd.c's ring and disc stood in for by
 * reads of the files: an SE bank through 0x200/0x202/0x20A/0x210/0x220,
 * and a music track through 0x204/0x208/0x218/0x228, including one played
 * past its loop point. They are written to build/port/iop/ as WAV files for
 * listening; nothing read from the files is printed.
 *
 *   spu2_test.exe [SE bank name, e.g. 6001] [music track id]
 */

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grx_spu2.h"
#include "grx_snd.h"
#include "grx_disc.h"
#include "../src/snd/snd_internal.h"

static int failures;
static int checks;

#define CHECK(c, ...)                                                   \
    do {                                                                \
        checks++;                                                       \
        if (!(c)) {                                                     \
            failures++;                                                 \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);                 \
            printf(__VA_ARGS__);                                        \
            printf("\n");                                               \
        }                                                               \
    } while (0)

void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("fatal: ");
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    exit(2);
}

/* ------------------------------------------------------------------------ */
/* Stand-ins for cdvd.c: the status block, one stream ring holding a file,
   and "the disc" as one file read by sector                               */

static uint8_t status[0x100];
static uint8_t *ring_data;
static uint32_t ring_len, ring_pos;
static FILE *disc_file;

void *grx_gsiop_status(void) {
    return status;
}

int grx_gsiop_strm_take(int slot, int n, void *dst) {
    uint32_t want = (uint32_t)n * 2048, i;

    (void)slot;
    for (i = 0; i < want; i++) {
        uint32_t p = ring_pos + i;
        if (dst != NULL) {
            ((uint8_t *)dst)[i] = p < ring_len ? ring_data[p] : 0;
        }
    }
    ring_pos += want;
    return n;
}

GrxDisc *grx_cdvd_disc(void) {
    return (GrxDisc *)&disc_file;
}

int grx_disc_read_sectors(GrxDisc *d, uint32_t lba, uint32_t count, void *dst) {
    size_t got = 0;

    (void)d;
    memset(dst, 0, (size_t)count * 2048);
    if (disc_file != NULL && fseek(disc_file, (long)lba * 2048, SEEK_SET) == 0) {
        got = fread(dst, 1, (size_t)count * 2048, disc_file);
    }
    (void)got;
    return (int)count;
}

/* ------------------------------------------------------------------------ */
/* Helpers                                                                  */

static void put_block(GrxSpu2 *s, uint32_t addr, int shift, int filter, int flags, const int nib[28]) {
    uint8_t b[16];
    int i;

    memset(b, 0, sizeof(b));
    b[0] = (uint8_t)(filter << 4 | shift);
    b[1] = (uint8_t)flags;
    for (i = 0; i < 28; i++) {
        b[2 + i / 2] |= (uint8_t)((nib[i] & 15) << ((i & 1) * 4));
    }
    grx_spu2_ram_write(s, addr, b, 16);
}

static void fill_nib(int nib[28], int v) {
    int i;
    for (i = 0; i < 28; i++) {
        nib[i] = v;
    }
}

/* A PS-ADPCM encoder (best filter and shift per block, by trial), for
   building test signals */
static void encode(const int16_t *pcm, int blocks, uint8_t *out) {
    static const int f0[5] = { 0, 60, 115, 98, 122 };
    static const int f1[5] = { 0, 0, -52, -55, -60 };
    int32_t h0 = 0, h1 = 0;
    int b;

    for (b = 0; b < blocks; b++) {
        const int16_t *x = pcm + b * 28;
        double best = 1e300;
        int bf = 0, bs = 0;
        int f, sh, i;
        uint8_t blk[16];

        for (f = 0; f < 5; f++) {
            for (sh = 0; sh <= 12; sh++) {
                int32_t a = h0, c = h1;
                double err = 0;
                for (i = 0; i < 28; i++) {
                    int32_t pred = (a * f0[f] + c * f1[f] + 32) >> 6;
                    int32_t q = 1 << (12 - sh);
                    int32_t r = x[i] - pred;
                    int32_t nib = (int32_t)floor((double)r / q + 0.5);
                    int32_t v;
                    if (nib > 7) nib = 7;
                    if (nib < -8) nib = -8;
                    v = nib * q + pred;
                    if (v > 32767) v = 32767;
                    if (v < -32768) v = -32768;
                    err += (double)(x[i] - v) * (x[i] - v);
                    c = a;
                    a = v;
                }
                if (err < best) {
                    best = err;
                    bf = f;
                    bs = sh;
                }
            }
        }
        memset(blk, 0, sizeof(blk));
        blk[0] = (uint8_t)(bf << 4 | bs);
        for (i = 0; i < 28; i++) {
            int32_t pred = (h0 * f0[bf] + h1 * f1[bf] + 32) >> 6;
            int32_t q = 1 << (12 - bs);
            int32_t nib = (int32_t)floor((double)(x[i] - pred) / q + 0.5);
            int32_t v;
            if (nib > 7) nib = 7;
            if (nib < -8) nib = -8;
            v = nib * q + pred;
            if (v > 32767) v = 32767;
            if (v < -32768) v = -32768;
            blk[2 + i / 2] |= (uint8_t)((nib & 15) << ((i & 1) * 4));
            h1 = h0;
            h0 = v;
        }
        memcpy(out + b * 16, blk, 16);
    }
}

static void wav_write(const char *path, const int16_t *lr, uint32_t frames) {
    FILE *f = fopen(path, "wb");
    uint32_t data = frames * 4, v32;
    uint16_t v16;

    if (f == NULL) {
        printf("  (can't write %s)\n", path);
        return;
    }
    fwrite("RIFF", 1, 4, f);
    v32 = 36 + data; fwrite(&v32, 4, 1, f);
    fwrite("WAVEfmt ", 1, 8, f);
    v32 = 16; fwrite(&v32, 4, 1, f);
    v16 = 1; fwrite(&v16, 2, 1, f);
    v16 = 2; fwrite(&v16, 2, 1, f);
    v32 = 48000; fwrite(&v32, 4, 1, f);
    v32 = 48000 * 4; fwrite(&v32, 4, 1, f);
    v16 = 4; fwrite(&v16, 2, 1, f);
    v16 = 16; fwrite(&v16, 2, 1, f);
    fwrite("data", 1, 4, f);
    fwrite(&data, 4, 1, f);
    fwrite(lr, 4, frames, f);
    fclose(f);
    printf("  wrote %s (%.1f s)\n", path, frames / 48000.0);
}

static double rms(const int16_t *lr, uint32_t frames) {
    double s = 0;
    uint32_t i;
    if (frames == 0) {
        return 0;
    }
    for (i = 0; i < frames * 2; i++) {
        s += (double)lr[i] * lr[i];
    }
    return sqrt(s / (frames * 2));
}

/* A voice on core c set up to play plainly: centre, full, dry */
static void plain_voice(GrxSpu2 *s, int c, int v, uint32_t ssa, uint16_t pitch) {
    uint32_t e = (uint32_t)GRX_SD_VOICE(v) | (uint32_t)c;

    grx_spu2_set_addr(s, GRX_SD_VA_SSA | e, ssa);
    grx_spu2_set_param(s, GRX_SD_VP_PITCH | e, pitch);
    grx_spu2_set_param(s, GRX_SD_VP_ADSR1 | e, 0x00FF);   /* attack at once, sustain at the top */
    grx_spu2_set_param(s, GRX_SD_VP_ADSR2 | e, 0x1FC0);   /* sustain holds */
    grx_spu2_set_param(s, GRX_SD_VP_VOLL | e, 0x3FFF);
    grx_spu2_set_param(s, GRX_SD_VP_VOLR | e, 0x3FFF);
}

static void plain_routing(GrxSpu2 *s) {
    grx_spu2_set_param(s, GRX_SD_P_MMIX | 0, 0x0C00);
    grx_spu2_set_param(s, GRX_SD_P_MMIX | 1, 0x0C0C);
    grx_spu2_set_switch(s, GRX_SD_S_VMIXL | 0, 0xFFFFFF);
    grx_spu2_set_switch(s, GRX_SD_S_VMIXR | 0, 0xFFFFFF);
    grx_spu2_set_switch(s, GRX_SD_S_VMIXL | 1, 0xFFFFFF);
    grx_spu2_set_switch(s, GRX_SD_S_VMIXR | 1, 0xFFFFFF);
    grx_spu2_set_param(s, GRX_SD_P_MVOLL | 0, 0x3FFF);
    grx_spu2_set_param(s, GRX_SD_P_MVOLR | 0, 0x3FFF);
    grx_spu2_set_param(s, GRX_SD_P_MVOLL | 1, 0x3FFF);
    grx_spu2_set_param(s, GRX_SD_P_MVOLR | 1, 0x3FFF);
}

/* ------------------------------------------------------------------------ */
/* Unit tests                                                               */

static void test_adpcm(void) {
    uint8_t b[16];
    int32_t h[2];
    int16_t o[28];
    int nib[28];
    int i, f;

    /* filter 0, shift 0: each nibble is the top 4 bits of the sample */
    for (i = 0; i < 28; i++) {
        nib[i] = i & 15;
    }
    memset(b, 0, 16);
    b[0] = 0x00;
    b[1] = 0x00;
    for (i = 0; i < 28; i++) {
        b[2 + i / 2] |= (uint8_t)(nib[i] << ((i & 1) * 4));
    }
    h[0] = h[1] = 0;
    f = grx_spu2_adpcm_block(b, h, o);
    CHECK(f == 0, "flags %d", f);
    CHECK(o[0] == 0 && o[1] == 4096 && o[7] == 28672 && o[8] == -32768 && o[15] == -4096 && o[17] == 4096,
          "filter 0 shift 0: %d %d %d %d %d %d", o[0], o[1], o[7], o[8], o[15], o[17]);
    CHECK(h[0] == o[27] && h[1] == o[26], "history after a block");

    /* shift 12, filter 1 (60/64 of the last sample), history 64 */
    memset(b, 0, 16);
    b[0] = 0x1C;
    b[1] = 0x06;
    b[2] = 0x01;            /* 1, 0 */
    b[3] = 0xF0;            /* 0, -1 */
    b[4] = 0x87;            /* 7, -8 */
    h[0] = 64;
    h[1] = 0;
    f = grx_spu2_adpcm_block(b, h, o);
    CHECK(f == 6, "flags byte returned: %d", f);
    CHECK(o[0] == 61 && o[1] == 57 && o[2] == 53 && o[3] == 49 && o[4] == 53 && o[5] == 42,
          "filter 1: %d %d %d %d %d %d", o[0], o[1], o[2], o[3], o[4], o[5]);

    /* filter 2 (115, -52) */
    memset(b, 0, 16);
    b[0] = 0x2C;
    h[0] = 100;
    h[1] = 50;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 139 && o[1] == 169 && o[2] == 191, "filter 2: %d %d %d", o[0], o[1], o[2]);

    /* the prediction rounds towards minus infinity (arithmetic shift) */
    memset(b, 0, 16);
    b[0] = 0x1C;
    h[0] = -100;
    h[1] = 0;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == -94, "negative prediction: %d", o[0]);

    /* filter 3 (98, -55) and 4 (122, -60) */
    memset(b, 0, 16);
    b[0] = 0x3C;
    h[0] = 1000;
    h[1] = 1000;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 672, "filter 3: %d", o[0]);   /* (98000 - 55000 + 32) >> 6 */
    b[0] = 0x4C;
    h[0] = 1000;
    h[1] = 1000;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 969, "filter 4: %d", o[0]);   /* (122000 - 60000 + 32) >> 6 */
    b[0] = 0x5C;                                /* filters past 4 act as 4 */
    h[0] = 1000;
    h[1] = 1000;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 969, "filter 5 as 4: %d", o[0]);

    /* shifts 13-15 act as 9 */
    memset(b, 0, 16);
    b[0] = 0x0D;
    b[2] = 0x01;
    h[0] = h[1] = 0;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 8, "shift 13: %d", o[0]);
    b[0] = 0x0F;
    h[0] = h[1] = 0;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 8, "shift 15: %d", o[0]);

    /* clamping */
    memset(b, 0, 16);
    b[0] = 0x40;
    b[2] = 0x07;
    h[0] = 32767;
    h[1] = 0;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == 32767, "clamp high: %d", o[0]);
    b[2] = 0x08;
    h[0] = -32768;
    h[1] = 0;
    grx_spu2_adpcm_block(b, h, o);
    CHECK(o[0] == -32768, "clamp low: %d", o[0]);
}

static void test_loop_flags(void) {
    GrxSpu2 *s = grx_spu2_create();
    int nib[28];
    int16_t out[2 * 128];
    uint32_t nax;

    plain_routing(s);
    fill_nib(nib, 1);
    put_block(s, 0x1000, 4, 0, GRX_SPU2_FLAG_LOOP_START, nib);
    fill_nib(nib, 2);
    put_block(s, 0x1010, 4, 0, 0, nib);
    fill_nib(nib, 3);
    put_block(s, 0x1020, 4, 0, GRX_SPU2_FLAG_END | GRX_SPU2_FLAG_REPEAT, nib);
    plain_voice(s, 1, 0, 0x1000, 0x1000);
    grx_spu2_set_switch(s, GRX_SD_S_ENDX | 1, 1);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
    CHECK(grx_spu2_get_switch(s, GRX_SD_S_ENDX | 1) == 0, "key on clears ENDX");

    grx_spu2_mix(s, out, 30);
    nax = grx_spu2_get_addr(s, GRX_SD_VA_NAX | 1) & ~15u;
    CHECK(nax == 0x1010, "after 30 samples at 0x1000 pitch: block %X", nax);
    grx_spu2_mix(s, out, 30);
    nax = grx_spu2_get_addr(s, GRX_SD_VA_NAX | 1) & ~15u;
    CHECK(nax == 0x1020, "after 60: block %X", nax);
    CHECK(grx_spu2_get_switch(s, GRX_SD_S_ENDX | 1) == 0, "no ENDX before the end block");
    grx_spu2_mix(s, out, 30);
    nax = grx_spu2_get_addr(s, GRX_SD_VA_NAX | 1) & ~15u;
    CHECK(nax == 0x1000, "after 90: back at the loop start, block %X", nax);
    CHECK(grx_spu2_get_switch(s, GRX_SD_S_ENDX | 1) == 1, "ENDX set at the end block");
    CHECK(grx_spu2_get_addr(s, GRX_SD_VA_LSAX | 1) == 0x1000, "LSA from the loop start flag");
    CHECK(grx_spu2_voice_phase(s, 1, 0) != GRX_SPU2_OFF, "END + REPEAT keeps playing");
    CHECK(out[2 * 29] > 0, "output while looping");

    /* END without REPEAT stops the voice */
    fill_nib(nib, 4);
    put_block(s, 0x2000, 12, 0, GRX_SPU2_FLAG_END, nib);
    plain_voice(s, 1, 1, 0x2000, 0x1000);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 2);
    grx_spu2_mix(s, out, 20);
    CHECK(grx_spu2_voice_phase(s, 1, 1) != GRX_SPU2_OFF, "playing before the end");
    grx_spu2_mix(s, out, 20);
    CHECK(grx_spu2_voice_phase(s, 1, 1) == GRX_SPU2_OFF, "END alone stops the voice");
    CHECK(grx_spu2_get_switch(s, GRX_SD_S_ENDX | 1) & 2, "ENDX for the stopped voice");
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_ENVX | GRX_SD_VOICE(1) | 1) == 0, "envelope 0 once stopped");

    /* a loop start set by software holds until a flag moves it */
    grx_spu2_set_addr(s, GRX_SD_VA_LSAX | GRX_SD_VOICE(1) | 1, 0x1010);
    CHECK(grx_spu2_get_addr(s, GRX_SD_VA_LSAX | GRX_SD_VOICE(1) | 1) == 0x1010, "LSAX write");
    grx_spu2_destroy(s);
}

static void test_envelope(void) {
    GrxSpu2Env e;
    GrxSpu2 *s;
    int i;
    int nib[28];
    int16_t out[2 * 16];
    uint32_t v0 = GRX_SD_VOICE(0) | 1;

    /* linear, fastest: +7 << 11 a sample */
    memset(&e, 0, sizeof(e));
    grx_spu2_env_start(&e, 0, 0, 0);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 14336, "attack rate 0, 1: %d", e.level);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 28672, "attack rate 0, 2: %d", e.level);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 0x7FFF, "attack rate 0, 3 (clamped): %d", e.level);

    /* rate 0x7F never moves */
    e.level = 1234;
    grx_spu2_env_start(&e, 0x7F, 0, 0);
    for (i = 0; i < 1000; i++) {
        grx_spu2_env_tick(&e);
    }
    CHECK(e.level == 1234, "rate 0x7F: %d", e.level);

    /* rate 48: shift 12, step 7 every second sample */
    e.level = 0;
    grx_spu2_env_start(&e, 48, 0, 0);
    for (i = 0; i < 10; i++) {
        grx_spu2_env_tick(&e);
    }
    CHECK(e.level == 35, "rate 48 after 10: %d", e.level);
    /* rate 51: shift 12, step 4 */
    e.level = 0;
    grx_spu2_env_start(&e, 51, 0, 0);
    for (i = 0; i < 10; i++) {
        grx_spu2_env_tick(&e);
    }
    CHECK(e.level == 20, "rate 51 after 10: %d", e.level);
    /* rate 60: shift 15, step 7 every 16 samples */
    e.level = 0;
    grx_spu2_env_start(&e, 60, 0, 0);
    for (i = 0; i < 64; i++) {
        grx_spu2_env_tick(&e);
    }
    CHECK(e.level == 28, "rate 60 after 64: %d", e.level);

    /* linear decrease: -8 + (rate & 3) */
    e.level = 1000;
    grx_spu2_env_start(&e, 44, 0, 1);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 992, "linear decrease rate 44: %d", e.level);
    e.level = 1000;
    grx_spu2_env_start(&e, 47, 0, 1);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 995, "linear decrease rate 47: %d", e.level);

    /* exponential decrease: the step scales with the level */
    e.level = 0x7FFF;
    grx_spu2_env_start(&e, 0, 1, 1);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 16383, "exp decrease 1: %d", e.level);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 8191, "exp decrease 2: %d", e.level);
    for (i = 0; i < 100; i++) {
        grx_spu2_env_tick(&e);
    }
    CHECK(e.level == 0, "exp decrease reaches 0: %d", e.level);

    /* exponential increase slows above 0x6000 */
    e.level = 0x1000;
    grx_spu2_env_start(&e, 0, 1, 0);
    grx_spu2_env_tick(&e);
    CHECK(e.level == 0x1000 + 14336, "exp increase below 0x6000: %d", e.level);
    e.level = 0x6000;
    grx_spu2_env_tick(&e);
    CHECK(e.level == 0x6000 + 3584, "exp increase above 0x6000: %d", e.level);

    /* A voice through its phases: attack 0, decay 0 to sustain level 7
       (0x4000), sustain holding, release 0 */
    s = grx_spu2_create();
    plain_routing(s);
    fill_nib(nib, 1);
    put_block(s, 0x3000, 12, 0, GRX_SPU2_FLAG_LOOP_START | GRX_SPU2_FLAG_END | GRX_SPU2_FLAG_REPEAT, nib);
    plain_voice(s, 1, 0, 0x3000, 0x1000);
    grx_spu2_set_param(s, GRX_SD_VP_ADSR1 | v0, 0x0007);
    grx_spu2_set_param(s, GRX_SD_VP_ADSR2 | v0, 0x1FC0);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
    CHECK(grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_ATTACK, "attack after key on");
    grx_spu2_mix(s, out, 3);
    CHECK(grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_DECAY, "decay after the attack: phase %d",
          grx_spu2_voice_phase(s, 1, 0));
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_ENVX | v0) == 0x7FFF, "ENVX at the peak");
    grx_spu2_mix(s, out, 1);
    CHECK(grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_SUSTAIN, "sustain at the level");
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_ENVX | v0) == 16383, "ENVX at sustain: %d",
          grx_spu2_get_param(s, GRX_SD_VP_ENVX | v0));
    grx_spu2_mix(s, out, 16);
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_ENVX | v0) == 16383, "sustain holds (rate 0x7F)");
    grx_spu2_set_switch(s, GRX_SD_S_KOFF | 1, 1);
    CHECK(grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_RELEASE, "release after key off");
    grx_spu2_mix(s, out, 1);
    CHECK(grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_OFF, "release done");

    /* the IRX's SE envelope: attack 0x1E (shift 7, step 5: +80 a
       sample), so the attack takes 0x7FFF / 80 = 410 samples */
    grx_spu2_set_param(s, GRX_SD_VP_ADSR1 | v0, 0x1EFF);
    grx_spu2_set_param(s, GRX_SD_VP_ADSR2 | v0, 0xDFED);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
    for (i = 0; i < 1000 && grx_spu2_voice_phase(s, 1, 0) == GRX_SPU2_ATTACK; i++) {
        grx_spu2_mix(s, out, 1);
    }
    CHECK(i == 410, "attack 0x1E: %d samples", i);
    grx_spu2_destroy(s);
}

static void test_volume(void) {
    GrxSpu2 *s = grx_spu2_create();
    int16_t out[2 * 8];
    uint32_t v0 = GRX_SD_VOICE(0) | 1;

    grx_spu2_set_param(s, GRX_SD_VP_VOLL | v0, 0x3FFF);
    CHECK((uint16_t)grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0) == 0x7FFE, "fixed volume: %X",
          grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0));
    grx_spu2_set_param(s, GRX_SD_VP_VOLL | v0, 0x4000);
    CHECK((int16_t)grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0) == -0x8000, "fixed negative volume");
    grx_spu2_set_param(s, GRX_SD_VP_VOLL | v0, 0);
    /* sweep: linear increase, rate 0 */
    grx_spu2_set_param(s, GRX_SD_VP_VOLL | v0, 0x8000);
    grx_spu2_mix(s, out, 1);
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0) == 14336, "sweep up 1: %d",
          grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0));
    grx_spu2_mix(s, out, 2);
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0) == 0x7FFF, "sweep up to the top");
    /* sweep: linear decrease, rate 0 */
    grx_spu2_set_param(s, GRX_SD_VP_VOLL | v0, 0xA000);
    grx_spu2_mix(s, out, 1);
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0) == 0x7FFF - 16384, "sweep down: %d",
          grx_spu2_get_param(s, GRX_SD_VP_VOLXL | v0));
    grx_spu2_destroy(s);
}

/* Frequency by zero crossings of the left channel */
static double frequency(const int16_t *lr, int frames) {
    int i, up = 0, first = -1, last = -1;
    for (i = 1; i < frames; i++) {
        if (lr[2 * (i - 1)] < 0 && lr[2 * i] >= 0) {
            if (first < 0) {
                first = i;
            }
            last = i;
            up++;
        }
    }
    if (up < 2) {
        return 0;
    }
    return (up - 1) * 48000.0 / (last - first);
}

static void test_pitch(void) {
    enum { BLOCKS = 240 };     /* 6720 samples: 140 cycles of 1 kHz at 48 kHz */
    static int16_t pcm[BLOCKS * 28];
    static uint8_t adpcm[BLOCKS * 16];
    static int16_t out[2 * 9600];
    const int16_t *g = grx_spu2_gauss_table();
    GrxSpu2 *s;
    int i, ok;
    double f, r;
    int16_t dc[4] = { 1000, 1000, 1000, 1000 };
    uint32_t v0 = GRX_SD_VOICE(0) | 1;

    /* the interpolation table: rising, every phase summing to nearly 1 */
    ok = 1;
    for (i = 1; i < 512; i++) {
        if (g[i] < g[i - 1]) {
            ok = 0;
        }
    }
    CHECK(ok, "gauss table rises");
    ok = 1;
    for (i = 0; i < 256; i++) {
        int sum = g[0xFF - i] + g[0x1FF - i] + g[0x100 + i] + g[i];
        if (sum > 0x7FFF || sum < 0x7F00) {
            ok = 0;
        }
        if (grx_spu2_interpolate(dc, i) < 990 || grx_spu2_interpolate(dc, i) > 1000) {
            ok = 0;
        }
    }
    CHECK(ok, "gauss: unity gain at every phase");
    {
        int16_t imp[4] = { 0, 1000, 0, 0 };
        int16_t imp2[4] = { 0, 0, 1000, 0 };
        CHECK(grx_spu2_interpolate(imp, 0) > grx_spu2_interpolate(imp, 128), "phase 0 is on s[1]");
        CHECK(abs(grx_spu2_interpolate(imp, 128) - grx_spu2_interpolate(imp2, 128)) <= 5,
              "phase 128 is between s[1] and s[2]");
        CHECK(grx_spu2_interpolate(imp2, 255) > grx_spu2_interpolate(imp2, 0), "phase 255 is near s[2]");
    }

    /* a 1 kHz sine, encoded, looping */
    for (i = 0; i < BLOCKS * 28; i++) {
        pcm[i] = (int16_t)lrint(8000.0 * sin(2 * M_PI * 1000.0 * i / 48000.0));
    }
    encode(pcm, BLOCKS, adpcm);
    adpcm[1] = GRX_SPU2_FLAG_LOOP_START;
    adpcm[(BLOCKS - 1) * 16 + 1] = GRX_SPU2_FLAG_END | GRX_SPU2_FLAG_REPEAT;

    s = grx_spu2_create();
    plain_routing(s);
    grx_spu2_ram_write(s, 0x10000, adpcm, sizeof(adpcm));
    {
        static const struct { uint16_t pitch; double hz; } t[] = {
            { 0x1000, 1000 }, { 0x0800, 500 }, { 0x2000, 2000 }, { 0x0400, 250 }, { 0x3FFF, 4000 },
        };
        int k;
        for (k = 0; k < (int)(sizeof(t) / sizeof(t[0])); k++) {
            plain_voice(s, 1, 0, 0x10000, t[k].pitch);
            grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
            grx_spu2_mix(s, out, 9600);
            f = frequency(out + 2 * 4800, 4800);
            CHECK(fabs(f - t[k].hz) < t[k].hz * 0.01, "pitch %04X: %.1f Hz, wanted %.0f", t[k].pitch, f, t[k].hz);
            if (k == 0) {
                /* level: 8000 * envelope 1 * voice 0x7FFE * master 0x7FFE,
                   rms of a sine 0.707; the interpolation's low pass takes
                   a little at 1 kHz */
                r = rms(out + 2 * 4800, 4800);
                CHECK(r > 8000 * 0.707 * 0.97 && r < 8000 * 0.707 * 1.01, "1 kHz level: rms %.0f", r);
            }
        }
    }
    /* the pitch counter: 0x800 moves a block in 56 samples */
    plain_voice(s, 1, 0, 0x10000, 0x0800);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
    grx_spu2_mix(s, out, 57);
    CHECK((grx_spu2_get_addr(s, GRX_SD_VA_NAX | v0) & ~15u) == 0x10010, "pitch 0x800: one block in 57 samples");
    grx_spu2_set_param(s, GRX_SD_VP_PITCH | v0, 0xFFFF);
    CHECK(grx_spu2_get_param(s, GRX_SD_VP_PITCH | v0) == 0x3FFF, "pitch is 14 bits");
    grx_spu2_destroy(s);
}

static void test_noise_and_routing(void) {
    GrxSpu2 *s = grx_spu2_create();
    static int16_t out[2 * 4800];
    int nib[28];
    int i, changes = 0;
    double r;

    plain_routing(s);
    /* noise on core 1 voice 0 (its sample data is a flat 0) */
    fill_nib(nib, 0);
    put_block(s, 0x4000, 0, 0, GRX_SPU2_FLAG_LOOP_START | GRX_SPU2_FLAG_END | GRX_SPU2_FLAG_REPEAT, nib);
    plain_voice(s, 1, 0, 0x4000, 0x1000);
    grx_spu2_set_core_attr(s, GRX_SD_C_NOISE_CLK | 1, 0x3F);
    CHECK(grx_spu2_get_core_attr(s, GRX_SD_C_NOISE_CLK | 1) == 0x3F, "noise clock");
    grx_spu2_set_switch(s, GRX_SD_S_NON | 1, 1);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 1, 1);
    grx_spu2_mix(s, out, 4800);
    for (i = 1; i < 4800; i++) {
        if (out[2 * i] != out[2 * i - 2]) {
            changes++;
        }
    }
    r = rms(out, 4800);
    CHECK(changes > 2000 && r > 3000, "noise: %d changes, rms %.0f", changes, r);
    grx_spu2_set_switch(s, GRX_SD_S_NON | 1, 0);
    grx_spu2_mix(s, out, 100);
    CHECK(out[2 * 99] == 0, "noise off: the flat sample");
    grx_spu2_set_switch(s, GRX_SD_S_KOFF | 1, 1);
    grx_spu2_mix(s, out, 100);

    /* core 0's voices reach the output only through core 1's Ext input */
    fill_nib(nib, 2);
    put_block(s, 0x5000, 0, 0, GRX_SPU2_FLAG_LOOP_START | GRX_SPU2_FLAG_END | GRX_SPU2_FLAG_REPEAT, nib);
    plain_voice(s, 0, 3, 0x5000, 0x1000);
    grx_spu2_set_switch(s, GRX_SD_S_KON | 0, 1u << 3);
    grx_spu2_mix(s, out, 100);
    CHECK(out[2 * 99] > 0 && out[2 * 99 + 1] > 0, "core 0 voice through core 1: %d", out[2 * 99]);
    grx_spu2_set_param(s, GRX_SD_P_MMIX | 1, 0x0C00);
    grx_spu2_mix(s, out, 100);
    CHECK(out[2 * 99] == 0, "core 1 without Ext: silent (%d)", out[2 * 99]);
    grx_spu2_set_param(s, GRX_SD_P_MMIX | 1, 0x0C0C);
    grx_spu2_set_param(s, GRX_SD_P_AVOLL | 1, 0);
    grx_spu2_mix(s, out, 100);
    CHECK(out[2 * 99] == 0 && out[2 * 99 + 1] > 0, "AVOLL 0 closes the left Ext");
    grx_spu2_set_switch(s, GRX_SD_S_VMIXR | 0, 0);
    grx_spu2_mix(s, out, 100);
    CHECK(out[2 * 99 + 1] == 0, "VMIXR off: no right dry");
    grx_spu2_destroy(s);
}

/* ------------------------------------------------------------------------ */
/* The driver with the game's files                                         */

static const char *iso_dir(void) {
    const char *e = getenv("ISO");
    return e != NULL && *e ? e : "iso";
}

static uint8_t *load(const char *rel, uint32_t *len) {
    char path[512];
    FILE *f;
    long n;
    uint8_t *p;

    snprintf(path, sizeof(path), "%s/%s", iso_dir(), rel);
    f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = malloc((size_t)n + 1);
    if (p == NULL || fread(p, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        free(p);
        return NULL;
    }
    p[n] = 0;
    fclose(f);
    *len = (uint32_t)n;
    return p;
}

/* MUSIC/VMSINIT.INI as SndDriverInit reads it: bank count, stream buffer
   count, track count, the banks' SPU addresses, the buffers' address and
   size */
typedef struct {
    int banks, bufs;
    uint32_t bank_addr[16];
    uint32_t buf_addr[4], buf_len[4];
} Ini;

static int read_ini(Ini *ini) {
    uint32_t len;
    char *t = (char *)load("MUSIC/VMSINIT.INI", &len);
    char *line, *p;
    int n = 0, i;
    int tracks = 0;

    memset(ini, 0, sizeof(*ini));
    if (t == NULL) {
        return -1;
    }
    for (p = t; *p; n++) {
        line = p;
        while (*p && *p != 13 && *p != 10) {
            p++;
        }
        while (*p == 13 || *p == 10) {
            *p++ = 0;
        }
        if (n == 0) ini->banks = atoi(line);
        else if (n == 1) ini->bufs = atoi(line);
        else if (n == 2) tracks = atoi(line);
        else if (n - 3 < ini->banks && n - 3 < 16) ini->bank_addr[n - 3] = (uint32_t)strtoul(line, NULL, 16);
        else if ((i = n - 3 - ini->banks) < ini->bufs && i < 4) {
            char *comma = strchr(line, ',');
            ini->buf_addr[i] = (uint32_t)strtoul(line, NULL, 16);
            ini->buf_len[i] = comma ? (uint32_t)strtoul(comma + 1, NULL, 16) : 0;
        }
    }
    (void)tracks;
    free(t);
    return ini->banks > 0 ? 0 : -1;
}

static void cmd(uint32_t c, int32_t a0, int32_t a1, int32_t a2) {
    int32_t a[3] = { a0, a1, a2 };
    int r = grx_snd_command(c, a, sizeof(a));
    CHECK(r == 1, "command %X taken", c);
}

static void driver_init(const Ini *ini) {
    int i;

    memset(status, 0, sizeof(status));
    cmd(0x200, 0, 0, 0);
    CHECK(status[0x50] | status[0x51] | status[0x52], "ready (+0x50) set by 0x200");
    for (i = 0; i < ini->banks; i++) {
        cmd(0x202, i, (int32_t)ini->bank_addr[i], 0);
    }
    for (i = 0; i < ini->bufs; i++) {
        cmd(0x204, i, (int32_t)ini->buf_addr[i], (int32_t)ini->buf_len[i]);
    }
}

static int16_t *grow(int16_t *buf, uint32_t *cap, uint32_t need) {
    if (need > *cap) {
        *cap = need * 2;
        buf = realloc(buf, (size_t)*cap * 4);
    }
    return buf;
}

static void test_se_bank(const char *name, const Ini *ini) {
    char rel[64], out_path[128];
    uint8_t *file;
    uint32_t len, frames = 0, cap = 0, i;
    int16_t *wav = NULL;
    int count, sectors, loud = 0, played = 0;
    const int bank = 0;

    snprintf(rel, sizeof(rel), "MUSIC/%s.SE", name);
    file = load(rel, &len);
    if (file == NULL) {
        printf("  (no %s/%s: skipping the SE bank)\n", iso_dir(), rel);
        return;
    }
    printf("SE bank %s\n", name);
    driver_init(ini);
    /* the EE opens a ring, reads the whole file, fetches its first sector */
    ring_data = file;
    ring_len = len;
    ring_pos = 2048;
    sectors = (int)((len + 2047) / 2048);
    count = (int)(file[8] | file[9] << 8 | file[10] << 16 | (uint32_t)file[11] << 24);
    cmd(0x20A, 0, bank, sectors);
    CHECK(status[0x30 + bank] == 2, "bankState %d after 0x20A", status[0x30 + bank]);
    CHECK(ring_pos == (uint32_t)sectors * 2048, "0x20A releases the rest of the ring");

    for (i = 0; i < (uint32_t)count && i < 127; i++) {
        const uint8_t *e = file + 2048 + i * 16;
        int16_t dur = (int16_t)(e[6] | e[7] << 8);
        uint32_t on = dur > 0 ? (uint32_t)dur * 800 : 48000;
        uint32_t start = frames, k;
        double r;

        if (dur == 0) {
            continue;
        }
        cmd(0x210, 0, bank << 16 | (int32_t)i, 0x3FFF << 16 | 0x3FFF);
        wav = grow(wav, &cap, frames + on + 24000);
        for (k = 0; k < on; k += 800) {
            uint32_t n = on - k < 800 ? on - k : 800;
            grx_snd_poll();
            grx_snd_render(wav + 2 * frames, (int)n);
            frames += n;
        }
        cmd(0x220, 0, 0, 0);
        grx_snd_render(wav + 2 * frames, 24000);
        frames += 24000;
        r = rms(wav + 2 * start, frames - start);
        played++;
        if (r > 50) {
            loud++;
        }
    }
    CHECK(played > 0 && loud > 0, "SE bank %s: %d of %d entries make sound", name, loud, played);
    printf("  %d entries, %d audible\n", played, loud);
    snprintf(out_path, sizeof(out_path), "build/port/iop/se_%s.wav", name);
    wav_write(out_path, wav, frames);
    cmd(0x20B, 0, bank, 0);
    CHECK(status[0x30 + bank] == 0, "0x20B releases the bank");
    free(wav);
    free(file);
    ring_data = NULL;
}

/* Plays a music track through the driver; returns the frames played */
static void test_music(const Ini *ini, int want_id, double seconds, int loop_test) {
    uint32_t len, frames = 0, total, cap = 0, i;
    uint8_t *idx = load("MUSIC/GRX.IDX", &len);
    int16_t *wav = NULL;
    char path[512], out_path[128];
    int best = -1;
    uint32_t best_lsn = 0, best_n = 0;
    int id = 0, started = 0, saw5 = 0, wraps = 0;
    uint32_t last_rpos = 0, hdr_samples = 0, rate = 0;
    double r;

    if (idx == NULL) {
        printf("  (no %s/MUSIC/GRX.IDX: skipping music)\n", iso_dir());
        return;
    }
    snprintf(path, sizeof(path), "%s/MUSIC/GRX.STZ", iso_dir());
    disc_file = fopen(path, "rb");
    if (disc_file == NULL) {
        printf("  (no %s: skipping music)\n", path);
        free(idx);
        return;
    }
    /* SndStrmIndexFind: bits 20-30 the id, 0-19 the sector, the next
       entry's sector ends it; with loop_test the shortest looping track */
    for (i = 0; i + 8 <= len; i += 4) {
        uint32_t v, nx, n;
        memcpy(&v, idx + i, 4);
        memcpy(&nx, idx + i + 4, 4);
        if (v == 0 || nx == 0) {
            break;
        }
        n = (nx & 0xFFFFF) - (v & 0xFFFFF);
        if (loop_test) {
            if ((v & 0x80000000u) && n >= 20 && n < 0x10000 && (best < 0 || n < best_n)) {
                best = (int)((v >> 20) & 0x7FF);
                best_lsn = v & 0xFFFFF;
                best_n = n;
            }
        } else if (best < 0 && (want_id <= 0 || (int)((v >> 20) & 0x7FF) == want_id)) {
            best = (int)((v >> 20) & 0x7FF);
            best_lsn = v & 0xFFFFF;
            best_n = n;
        }
    }
    free(idx);
    if (best < 0) {
        printf("  (no such track)\n");
        fclose(disc_file);
        disc_file = NULL;
        return;
    }
    id = best;
    {
        uint8_t h[2048];
        grx_disc_read_sectors(NULL, best_lsn, 1, h);
        memcpy(&rate, h + 4, 4);
        memcpy(&hdr_samples, h + 0x1C, 4);
    }
    if (loop_test && rate > 0) {
        seconds = (double)hdr_samples / rate + 4.0;
    }
    printf("music track %d%s (%u sectors)\n", id, loop_test ? ", past its loop" : "", best_n);
    driver_init(ini);
    cmd(0x208, 0, (int32_t)best_lsn, (int32_t)best_n);
    CHECK(status[0x40] == 1, "strmInfo state 1 after 0x208: %d", status[0x40]);
    total = (uint32_t)(seconds * 48000);
    wav = grow(wav, &cap, total + 800);
    while (frames < total) {
        int st;
        grx_snd_poll();
        grx_snd_render(wav + 2 * frames, 800);
        frames += 800;
        st = (int8_t)status[0x40];
        if (!started && st >= 3 && st <= 5) {
            cmd(0x218, 0, 0, 0);
            started = 1;
        }
        if (st == 5) {
            saw5 = 1;
        }
        if (snd.vbs[0].rpos < last_rpos) {
            wraps++;
        }
        last_rpos = snd.vbs[0].rpos;
    }
    grx_snd_poll();
    CHECK(saw5, "the stream reached state 5 (playing)");
    CHECK(status[0x41] >= 1 && status[0x41] <= 8, "strmInfo channels %d", status[0x41]);
    r = rms(wav + 2 * (frames / 2), frames / 2);
    CHECK(r > 200, "music is audible: rms %.0f", r);
    if (loop_test && frames > 96000) {
        r = rms(wav + 2 * (frames - 96000), 96000);
        CHECK(wraps >= 1 && (int8_t)status[0x40] == 5 && r > 200,
              "looped: %d wraps, state %d, rms of the last 2 s %.0f", wraps, (int8_t)status[0x40], r);
    }
    printf("  rms %.0f, %d loop wraps\n", rms(wav, frames), wraps);
    snprintf(out_path, sizeof(out_path), "build/port/iop/bgm_%d.wav", id);
    wav_write(out_path, wav, frames);

    /* stop: state 0, the voices released */
    cmd(0x228, 0, 0, 0);
    CHECK(status[0x40] == 0, "strmInfo state 0 after 0x228");
    grx_snd_render(wav, 48000);
    r = rms(wav + 2 * 24000, 24000);
    CHECK(r < 5, "silent after the stop: rms %.1f", r);
    free(wav);
    fclose(disc_file);
    disc_file = NULL;
}

static void test_driver_unit(void) {
    /* commands outside 0x2xx are not the driver's */
    int32_t a[3] = { 0, 0, 0 };
    CHECK(grx_snd_command(0x101, a, 12) == 0, "0x101 is not a sound command");
    CHECK(grx_snd_command(0x300, a, 12) == 0, "0x300 is not a sound command");
    CHECK(grx_snd_command(0x2FF, a, 12) == 1, "unknown 0x2xx is taken");
}

int main(int argc, char **argv) {
    Ini ini;
    const char *bank = argc > 1 ? argv[1] : "6001";
    int track = argc > 2 ? atoi(argv[2]) : 0;

    test_adpcm();
    test_loop_flags();
    test_envelope();
    test_volume();
    test_pitch();
    test_noise_and_routing();
    printf("unit tests: %d checks, %d failures\n", checks, failures);

    if (grx_snd_init() != 0) {
        printf("FAIL: grx_snd_init\n");
        return 1;
    }
    grx_snd_set_realtime(1);   /* the test renders itself */
    test_driver_unit();
    if (read_ini(&ini) == 0) {
        test_se_bank(bank, &ini);
        test_music(&ini, track, 20.0, 0);
        test_music(&ini, 0, 0, 1);
    } else {
        printf("(no %s/MUSIC/VMSINIT.INI: skipping the driver tests on the game's files)\n", iso_dir());
    }
    grx_snd_quit();
    printf("%s (%d checks, %d failures)\n", failures ? "FAILED" : "all ok", checks, failures);
    return failures != 0;
}
