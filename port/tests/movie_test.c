/*
 * Standalone test of the movie replacement (grx_movie.h):
 *
 *  1. The .MOV split (mov.c): every PSS sector of the file, in order,
 *     must make one unbroken program stream (pack headers and PES
 *     packets back to back) that ends with the end code exactly at the
 *     header's PSS byte count; the VBS sectors must add up to its VBS
 *     count.
 *  2. The video decoder (mpeg2.c): decodes the first seconds of the movie
 *     (--skip N first passes over N frames) and writes every --every'th
 *     frame as PPM (RGB through the IPU's colour conversion) into
 *     build/port/movie/ (gitignored). Checks the frame count, the picture
 *     types, the display order and the speed (frames per second, decode
 *     and colour conversion; the movies are 30 fps). Then the whole movie,
 *     to its sequence end, without a corrupt slice.
 *     --yuv FILE writes the frames as raw yuv420p, to compare with a
 *     reference decoder: against ffmpeg's decoding of the same video
 *     stream, GRM01 frames 1500-1619 come out at 67-70 dB PSNR (the IDCTs
 *     round differently; nothing else differs).
 *  3. The audio: the VBS stream (PS-ADPCM, two channels interleaved by
 *     0x800 bytes after the header sector), decoded with the SPU2's ADPCM
 *     decoder; the first seconds go to a WAV. Checks the header and that
 *     both channels are as smooth across sector boundaries as inside
 *     sectors (the interleave is right).
 *  4. libmpeg's replacement (libmpeg.c): sceMpegCreate/GetPicture for the
 *     whole movie, with the bitstream fed through an emulated toIPU DMA
 *     chain laid out as the game's viBuf builds it (2 KB sectors, ref
 *     tags, a refe after the last full sector, next back to the start;
 *     viBufAddDMA transcribed), sectors added from the nodata callback,
 *     the time stamp callback, the output in the IPU's macroblock order.
 *     Its frames must equal the direct decoder's byte for byte, and
 *     sceMpegIsEnd must come after the last one.
 *
 * Nothing from the disc is written anywhere but build/port/movie/ (or
 * --frames-out DIR).
 *
 * Build (from the repository root; scratch is any directory):
 *   CLANG=<llvm-mingw's i686-w64-mingw32-clang.exe, as in port/docs/disc.md>
 *   "$CLANG" -std=gnu11 -Wall -O2 -msse2 -mfpmath=sse -Iport/include -Iinclude \
 *       port/tests/movie_test.c port/src/movie/mpeg2.c port/src/movie/mov.c \
 *       port/src/movie/libmpeg.c port/src/spu2/spu2.c port/src/hwmem.c \
 *       -o <scratch>/movie_test.exe
 *   <scratch>/movie_test.exe [iso/MOVIE/GRM01.MOV] [--seconds N] [--skip N] [--every N]
 *                            [--yuv FILE] [--frames-out DIR]
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grx_movie.h"
#include "grx_spu2.h"

static int failures;

#define CHECK(cond, ...)                                                                                  \
    do {                                                                                              \
        if (!(cond)) {                                                                                \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);                                               \
            printf(__VA_ARGS__);                                                                      \
            printf("\n");                                                                             \
            failures++;                                                                               \
        }                                                                                             \
    } while (0)

static double now_s(void) {
    struct timespec t;
    timespec_get(&t, TIME_UTC);
    return (double)t.tv_sec + t.tv_nsec * 1e-9;
}

/* ------------------------------------------------------------------------ */
/* The file                                                                 */

static uint8_t *mov;
static size_t mov_size;

static int load(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    _fseeki64(f, 0, SEEK_END);
    mov_size = (size_t)_ftelli64(f);
    _fseeki64(f, 0, SEEK_SET);
    mov = malloc(mov_size);
    if (mov == NULL || fread(mov, 1, mov_size, f) != mov_size) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

static uint8_t *pss, *vbs;
static size_t pss_len, vbs_len;

static void split(void) {
    GrxMovSplit s;
    size_t i, n = mov_size / 2048;

    grx_mov_split_init(&s, mov);
    pss = malloc(mov_size);
    vbs = malloc(mov_size);
    for (i = 0; i < n; i++) {
        if (grx_mov_split_next(&s) == 0) {
            memcpy(pss + pss_len, mov + i * 2048, 2048);
            pss_len += 2048;
        } else {
            memcpy(vbs + vbs_len, mov + i * 2048, 2048);
            vbs_len += 2048;
        }
    }
    printf("split: header PSS %d bytes, VBS %d bytes; intPSS %d intVBS %d, %d chunks; %zu PSS sectors, %zu VBS\n",
           s.pss_bytes, s.vbs_bytes, s.int_pss, s.int_vbs, s.time, pss_len / 2048, vbs_len / 2048);
}

/* The program stream's video payloads; checks the stream is unbroken */
static uint8_t *es;
static size_t es_len;

static void demux(void) {
    GrxMovSplit s;
    size_t o = 0;
    int packs = 0, pes = 0, ended = 0;

    grx_mov_split_init(&s, mov);
    es = malloc(pss_len);
    while (o + 4 <= pss_len) {
        const uint8_t *p = pss + o;
        if (p[0] != 0 || p[1] != 0 || p[2] != 1) {
            break;
        }
        if (p[3] == 0xBA) {
            o += 14 + (p[13] & 7);
            packs++;
        } else if (p[3] == 0xB9) {
            o += 4;
            ended = 1;
            break;
        } else {
            size_t len = (size_t)p[4] << 8 | p[5];
            if (p[3] == 0xE0) {
                size_t h = 9 + p[8];
                memcpy(es + es_len, p + h, len + 6 - h);
                es_len += len + 6 - h;
            }
            o += 6 + len;
            pes++;
        }
    }
    printf("demux: %d packs, %d PES packets, %zu bytes of video, stream %s at %zu\n", packs, pes, es_len,
           ended ? "ends" : "BREAKS", o);
    CHECK(ended, "the PSS stream breaks at byte %zu", o);
    CHECK((int64_t)o == s.pss_bytes, "the end code is at %zu, the header says %d", o, s.pss_bytes);
    CHECK((int64_t)vbs_len >= s.vbs_bytes && (int64_t)vbs_len - s.vbs_bytes < 2048 * 4,
          "VBS %zu bytes, the header says %d", vbs_len, s.vbs_bytes);
}

/* ------------------------------------------------------------------------ */
/* Output files                                                             */

static void write_ppm(const char *path, const uint8_t *rgba, int w, int h) {
    FILE *f = fopen(path, "wb");
    int i;
    if (f == NULL) {
        printf("can't write %s\n", path);
        failures++;
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (i = 0; i < w * h; i++) {
        fwrite(rgba + i * 4, 1, 3, f);
    }
    fclose(f);
}

static void put16(FILE *f, int v) {
    fputc(v & 0xFF, f);
    fputc((v >> 8) & 0xFF, f);
}
static void put32(FILE *f, uint32_t v) {
    put16(f, (int)(v & 0xFFFF));
    put16(f, (int)(v >> 16));
}

/* ------------------------------------------------------------------------ */
/* 2. The decoder                                                           */

typedef struct {
    const uint8_t *p;
    size_t len, pos;
} Mem;

static int mem_read(void *ctx, uint8_t *dst, int max) {
    Mem *m = ctx;
    size_t n = m->len - m->pos;
    if (n == 0) {
        return -1;
    }
    if (n > (size_t)max) {
        n = (size_t)max;
    }
    memcpy(dst, m->p + m->pos, n);
    m->pos += n;
    return (int)n;
}

static uint64_t fnv(const uint8_t *p, size_t n, uint64_t h) {
    size_t i;
    for (i = 0; i < n; i++) {
        h = (h ^ p[i]) * 0x100000001B3ULL;
    }
    return h;
}

#define MAX_HASH 4000
static uint64_t frame_hash[MAX_HASH];
static int frame_count;

static char base_name[64] = "movie";

static void test_decoder(int skip, int frames, int every, const char *outdir, const char *yuv_path) {
    Mem src = {es, es_len, 0};
    GrxMpeg2 *m = grx_mpeg2_create(mem_read, &src);
    GrxMpeg2Frame f;
    FILE *yuv = yuv_path ? fopen(yuv_path, "wb") : NULL;
    uint8_t *rgba = NULL, *mbs = NULL;
    int n = 0, types[4] = {0, 0, 0, 0}, r = 1, last_tr = -1, order_ok = 1;
    double t0, t_dec = 0, t_csc = 0;

    while (n < skip + frames) {
        double t1;
        t0 = now_s();
        r = grx_mpeg2_decode(m, &f);
        t1 = now_s();
        t_dec += t1 - t0;
        if (r != 1) {
            break;
        }
        if (rgba == NULL) {
            rgba = malloc((size_t)f.width * f.height * 4);
            mbs = malloc((size_t)f.width * f.height * 4);
            printf("decoder: %dx%d (coded %dx%d)\n", f.display_width, f.display_height, f.width, f.height);
        }
        types[f.type & 3]++;
        /* display order: temporal_reference counts up within a GOP */
        if (f.temporal_reference != 0 && f.temporal_reference != last_tr + 1) {
            order_ok = 0;
        }
        last_tr = f.temporal_reference;
        t0 = now_s();
        grx_mpeg2_csc_mb(&f, mbs, (f.width / 16) * (f.height / 16));
        t_csc += now_s() - t0;
        if (n < MAX_HASH) {
            frame_hash[n] = fnv(mbs, (size_t)f.width * f.height * 4, 0xCBF29CE484222325ULL);
        }
        if (yuv != NULL && n >= skip) {
            int y;
            for (y = 0; y < f.display_height; y++) {
                fwrite(f.y + y * f.y_stride, 1, (size_t)f.display_width, yuv);
            }
            for (y = 0; y < f.display_height / 2; y++) {
                fwrite(f.cb + y * f.c_stride, 1, (size_t)f.display_width / 2, yuv);
            }
            for (y = 0; y < f.display_height / 2; y++) {
                fwrite(f.cr + y * f.c_stride, 1, (size_t)f.display_width / 2, yuv);
            }
        }
        if (outdir != NULL && every > 0 && n >= skip && (n - skip) % every == 0) {
            char path[512];
            grx_mpeg2_csc_linear(&f, rgba, f.width * 4);
            snprintf(path, sizeof(path), "%s/%s_%04d.ppm", outdir, base_name, n);
            write_ppm(path, rgba, f.display_width, f.display_height);
        }
        n++;
    }
    frame_count = n;
    printf("decoder: %d frames (I %d, P %d, B %d), %d corrupt slices, %.1f ms decode + %.1f ms CSC per frame: "
           "%.0f frames/s\n",
           n, types[1], types[2], types[3], grx_mpeg2_errors(m), 1000 * t_dec / (n ? n : 1),
           1000 * t_csc / (n ? n : 1), n / (t_dec + t_csc));
    CHECK(n == skip + frames, "decoded %d of %d frames (last result %d)", n, skip + frames, r);
    CHECK(grx_mpeg2_errors(m) == 0, "%d corrupt slices", grx_mpeg2_errors(m));
    CHECK(types[1] > 0 && types[2] > 0 && types[3] > 0, "picture types I %d P %d B %d", types[1], types[2],
          types[3]);
    CHECK(order_ok, "temporal_reference out of display order");
    CHECK(n / (t_dec + t_csc) >= 60.0, "%.0f frames/s: less than twice real time", n / (t_dec + t_csc));
    if (yuv != NULL) {
        fclose(yuv);
    }
    free(rgba);
    free(mbs);
    grx_mpeg2_destroy(m);
}

/* The whole movie: every picture decodes, and the sequence end comes */
static int whole_count;

static void test_whole(void) {
    Mem src = {es, es_len, 0};
    GrxMpeg2 *m = grx_mpeg2_create(mem_read, &src);
    GrxMpeg2Frame f;
    int n = 0, r;
    double t0 = now_s();

    while ((r = grx_mpeg2_decode(m, &f)) == 1) {
        n++;
    }
    whole_count = n;
    printf("whole movie: %d frames, %d corrupt slices, ended %d (result %d), %.0f frames/s\n", n,
           grx_mpeg2_errors(m), grx_mpeg2_ended(m), r, n / (now_s() - t0));
    CHECK(r == 0 && grx_mpeg2_ended(m), "no clean sequence end (result %d)", r);
    CHECK(grx_mpeg2_errors(m) == 0, "%d corrupt slices", grx_mpeg2_errors(m));
    grx_mpeg2_destroy(m);
}

/* ------------------------------------------------------------------------ */
/* 3. The audio                                                             */

static void test_audio(double seconds, const char *wav_path) {
    int ch = vbs[0] | vbs[1] << 8, rate = vbs[4] | vbs[5] << 8 | vbs[6] << 16;
    int il = vbs[8] | vbs[9] << 8 | vbs[10] << 16;
    size_t wav_frames = (size_t)(seconds * rate), npairs, pair, frames = 0;
    int16_t *pcm;
    int32_t hist[2][2] = {{0, 0}, {0, 0}};
    double jump = 0, inner = 0;
    long njump = 0, ninner = 0;
    FILE *f;

    printf("audio: %d channels, %d Hz, interleave 0x%X\n", ch, rate, il);
    CHECK(ch == 2 && rate == 48000 && il == 0x800, "VBS header %d ch %d Hz 0x%X", ch, rate, il);
    if (ch != 2 || il != 0x800) {
        return;
    }
    /* the whole stream: L sector, R sector, ... after the header sector */
    npairs = (vbs_len - 2048) / 4096;
    pcm = malloc(npairs * 2048 / 16 * 28 * 2 * sizeof(int16_t));
    for (pair = 0; pair < npairs; pair++) {
        int16_t out[2][128 * 28];
        int c, b;
        for (c = 0; c < 2; c++) {
            const uint8_t *p = vbs + 2048 + (pair * 2 + c) * 2048;
            for (b = 0; b < 128; b++) {
                grx_spu2_adpcm_block(p + b * 16, hist[c], out[c] + b * 28);
            }
            /* how smooth: the step from one sector into the next against
               the steps inside sectors (a wrong interleave shows as jumps) */
            if (frames > 0) {
                jump += abs(out[c][0] - pcm[(frames - 1) * 2 + c]);
                njump++;
            }
            for (b = 1; b < 128 * 28; b++) {
                inner += abs(out[c][b] - out[c][b - 1]);
                ninner++;
            }
        }
        for (b = 0; b < 128 * 28; b++) {
            pcm[frames * 2] = out[0][b];
            pcm[frames * 2 + 1] = out[1][b];
            frames++;
        }
    }
    printf("audio: %zu frames (%.1f s); mean step %.1f inside sectors, %.1f across them\n", frames,
           (double)frames / rate, inner / ninner, jump / njump);
    CHECK(jump / njump < 2 * inner / ninner + 16, "the steps across sectors (%.1f) are much bigger than "
          "inside them (%.1f): the interleave is wrong", jump / njump, inner / ninner);
    f = wav_path ? fopen(wav_path, "wb") : NULL;
    if (f != NULL) {
        uint32_t n = (uint32_t)(wav_frames < frames ? wav_frames : frames), bytes = n * 4;
        fwrite("RIFF", 1, 4, f);
        put32(f, 36 + bytes);
        fwrite("WAVEfmt ", 1, 8, f);
        put32(f, 16);
        put16(f, 1);
        put16(f, 2);
        put32(f, (uint32_t)rate);
        put32(f, (uint32_t)rate * 4);
        put16(f, 4);
        put16(f, 16);
        fwrite("data", 1, 4, f);
        put32(f, bytes);
        fwrite(pcm, 4, n, f);
        fclose(f);
    }
    free(pcm);
}

/* ------------------------------------------------------------------------ */
/* 4. libmpeg over the emulated toIPU chain                                 */

/* libmpeg.c's calls, as the game declares them */
typedef struct TestMpeg {
    int32_t width, height, frameCount, pad;
    int64_t pts, dts, flags, pts2nd, dts2nd, flags2nd;
    void *sys;
} TestMpeg;
int32_t sceMpegInit(void);
int32_t sceMpegCreate(TestMpeg *mp, uint32_t work, uint32_t size);
int32_t sceMpegDelete(TestMpeg *mp);
void *sceMpegAddCallback(TestMpeg *mp, int32_t type, void *func, void *data);
int32_t sceMpegGetPicture(TestMpeg *mp, int32_t rgb32, int32_t mbcount);
int32_t sceMpegIsEnd(TestMpeg *mp);
int32_t sceMpegIsRefBuffEmpty(TestMpeg *mp);
void sceMpegReset(TestMpeg *mp);
int grx_hwmem_init(void);
void *grx_hwmem_alloc_low(size_t size);

#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))

/* The game's viBuf (466458-467368), transcribed: the ring of n 2 KB
   sectors, the tag list, dmaStart/dmaN (sectors given to the DMA) and
   readBytes (bytes put but not yet given) */
static struct {
    uint8_t *data;
    uint64_t *tag;     /* 2 words per tag */
    int n, dmaStart, dmaN, readBytes, buffSize;
} vb;

static void scTag2(uint64_t *t, uint32_t addr, int id, int qwc) {
    t[0] = ((uint64_t)addr << 32) | ((uint64_t)id << 28) | (uint32_t)qwc;
}

static void setD4_CHCR(uint32_t v) {
    REG(0x1000B400) = v;
}

static uint32_t getFIFOindex(uint32_t madr) {
    if (madr == (((uint32_t)vb.n * 16 + (uint32_t)(uintptr_t)vb.tag + 16) & 0x0FFFFFFF)) {
        return 0;
    }
    return (madr - (uint32_t)(uintptr_t)vb.data) >> 11;
}

static void viBufReset(void) {
    int i;
    vb.dmaStart = vb.dmaN = vb.readBytes = 0;
    for (i = 0; i < vb.n; i++) {
        scTag2(vb.tag + i * 2, (uint32_t)(uintptr_t)(vb.data + (i << 11)), 3, 0x80);
    }
    scTag2(vb.tag + i * 2, (uint32_t)(uintptr_t)vb.tag & 0x0FFFFFFF, 2, 0);
    REG(0x1000B420) = 0;
    REG(0x1000B410) = (uint32_t)(uintptr_t)vb.data & 0x0FFFFFFF;
    REG(0x1000B430) = (uint32_t)(uintptr_t)vb.tag & 0x0FFFFFFF;
    setD4_CHCR(5);
}

static void viBufAddDMA(void) {
    int kick = 0, n, i, j, idx, done, tail, last, pos, cnt, bytes;
    uint32_t madr;

    setD4_CHCR(5);
    madr = REG(0x1000B400);
    pos = (int)getFIFOindex(REG(0x1000B410));
    cnt = vb.n;
    done = (pos + cnt - vb.dmaStart) % cnt;
    vb.dmaStart = (vb.dmaStart + done) % cnt;
    vb.dmaN -= done;
    tail = vb.dmaStart + vb.dmaN;
    idx = tail % cnt;
    bytes = vb.readBytes;
    n = bytes / 2048;
    vb.readBytes = bytes - n * 2048;
    if (n > 0) {
        last = (tail - 1 + cnt) % cnt;
        scTag2(vb.tag + last * 2, (uint32_t)(uintptr_t)(vb.data + last * 2048), 3, 0x80);
        kick = 1;
    }
    j = idx;
    for (i = 0; i < n; i++) {
        scTag2(vb.tag + j * 2, (uint32_t)(uintptr_t)(vb.data + j * 2048), i == n - 1 ? 0 : 3, 0x80);
        j = (j + 1) % vb.n;
    }
    vb.dmaN += n;
    if (vb.dmaN != 0) {
        if (kick) {
            madr = (madr & 0x0FFFFFFF) | 0x30000000;
        }
        setD4_CHCR(madr | 0x100);
    }
}

/* viBufBeginPut/EndPut with the ES as the source, in PES-sized pieces */
static const uint8_t *feed_es;
static size_t feed_len, feed_pos;
static int feed_flushed, nodata_calls, ts_calls;

static void feed(void) {
    int avail = ((vb.n - (vb.dmaN + 2)) << 11) - vb.readBytes;
    int off = (((vb.dmaStart + vb.dmaN) << 11) + vb.readBytes) % vb.buffSize;
    int n = avail < 0xFF2 ? avail : 0xFF2, k;

    if (n < 0) {
        n = 0;
    }
    if ((size_t)n > feed_len - feed_pos) {
        n = (int)(feed_len - feed_pos);
    }
    for (k = 0; k < n; k++) {
        vb.data[(off + k) % vb.buffSize] = feed_es[feed_pos + k];
    }
    feed_pos += (size_t)n;
    vb.readBytes += n;
    if (feed_pos == feed_len && !feed_flushed) {
        /* videoDecFlush: the end code is in the ES already; viBufFlush
           rounds up to a whole sector */
        int pad = (vb.readBytes + 0x7FF) / 0x800 * 0x800 - vb.readBytes;
        int off2 = (((vb.dmaStart + vb.dmaN) << 11) + vb.readBytes) % vb.buffSize;
        for (k = 0; k < pad; k++) {
            vb.data[(off2 + k) % vb.buffSize] = 0;
        }
        vb.readBytes += pad;
        feed_flushed = 1;
    }
}

static int32_t cb_nodata(void *mp, void *cbd, void *data) {
    (void)mp, (void)cbd, (void)data;
    nodata_calls++;
    feed();
    viBufAddDMA();
    return 1;
}

static int32_t cb_ts(void *mp, void *cbd, void *data) {
    int64_t *t = (int64_t *)((uint8_t *)cbd + 8);
    uint32_t bp = REG(0x10002020);
    (void)mp, (void)data;
    ts_calls++;
    /* where the decoder is, as viBufGetTs works it out */
    t[0] = (int64_t)REG(0x1000B410) - (int64_t)((((bp >> 16) & 3) + ((bp >> 8) & 0xF)) * 16);
    t[1] = -1;
    return 1;
}

static uint8_t *lm_work, *lm_out;

/* First thing: the game's buffers are below 0x08000000 (the movie file
   read later fills that space up) */
static void libmpeg_alloc(void) {
    grx_hwmem_init();      /* the IOP block may be taken; the registers are what's needed */
    vb.n = 0x80;
    vb.buffSize = vb.n << 11;
    vb.data = grx_hwmem_alloc_low((size_t)vb.buffSize);
    vb.tag = grx_hwmem_alloc_low((size_t)(vb.n + 1) * 16);
    lm_work = grx_hwmem_alloc_low(0x13B800);
    lm_out = grx_hwmem_alloc_low(0x118000);
}

static void test_libmpeg(const uint8_t *src, size_t len, int frames) {
    TestMpeg mp;
    uint8_t *work = lm_work, *out = lm_out;
    int n = 0, same = 0, r = 0;
    double t0;

    if (!vb.data || !vb.tag || !work || !out || (uintptr_t)out >= 0x08000000u) {
        printf("libmpeg: no low memory\n");
        failures++;
        return;
    }
    feed_es = src;
    feed_len = len;
    feed_pos = 0;
    sceMpegInit();
    CHECK(sceMpegCreate(&mp, (uint32_t)(uintptr_t)work, 0x13B800) == 1, "sceMpegCreate");
    sceMpegAddCallback(&mp, 1, (void *)cb_nodata, NULL);
    sceMpegAddCallback(&mp, 5, (void *)cb_ts, NULL);
    viBufReset();
    t0 = now_s();
    while ((frames < 0 || n < frames) && !sceMpegIsEnd(&mp)) {
        r = sceMpegGetPicture(&mp, (int32_t)(uintptr_t)out, 0x460);
        if (r < 0) {
            break;
        }
        if (n < MAX_HASH && fnv(out, 0x118000, 0xCBF29CE484222325ULL) == frame_hash[n]) {
            same++;
        }
        n++;
    }
    printf("libmpeg: %d pictures through the toIPU chain (%d the same as the direct decoder's), "
           "frameCount %d, %d nodata calls, %d time stamp calls, %.0f frames/s\n",
           n, same, mp.frameCount, nodata_calls, ts_calls, n / (now_s() - t0));
    CHECK(r == 0 && n == (frames < 0 ? whole_count : frames), "sceMpegGetPicture %d after %d pictures", r, n);
    CHECK(frames >= 0 || (sceMpegIsEnd(&mp) && sceMpegIsRefBuffEmpty(&mp)), "no end after %d pictures", n);
    CHECK(same == (n < frame_count ? n : frame_count), "%d of %d pictures differ from the direct decoder's",
          (n < frame_count ? n : frame_count) - same, n < frame_count ? n : frame_count);
    CHECK(mp.frameCount == n && mp.width == 640 && mp.height == 448, "frameCount %d, %dx%d", mp.frameCount,
          mp.width, mp.height);
    CHECK(ts_calls >= n, "%d time stamp calls for %d pictures", ts_calls, n);
    sceMpegDelete(&mp);
}

/* The platform's log, for libmpeg.c */
void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

/* ------------------------------------------------------------------------ */
/* 5. The IOP side (iop_movie.c over cdvd.c), with -DWITH_IOP and a disc    */

#ifdef WITH_IOP
#include "grx_disc.h"

/* The sound driver's side of the VBS stream, as a fake: takes sectors
   until 256 are queued (its read-ahead), and plays one a round */
static uint8_t *snd_got;
static size_t snd_len;
static int snd_queued;

int grx_snd_strm_feed(int stream, const void *sectors, int n) {
    if (stream != 0 || snd_queued >= 256) {
        return 0;
    }
    if (n > 256 - snd_queued) {
        n = 256 - snd_queued;
    }
    memcpy(snd_got + snd_len, sectors, (size_t)n * 2048);
    snd_len += (size_t)n * 2048;
    snd_queued += n;
    return n;
}

/* The rest of the sound driver, which cdvd.c calls: nothing */
int grx_snd_command(uint32_t cmd, const void *args, uint32_t size) {
    (void)args, (void)size;
    return (cmd & 0xF00) == 0x200;
}

void grx_snd_poll(void) {
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    exit(3);
}

/* cdvd.c's dispatch with the movie side's hook in front, as cdvd.c is
   asked to do it */
static uint16_t seq;

static void command(int16_t cmd, int32_t a0, int32_t a1, int32_t a2) {
    int32_t a[3] = {a0, a1, a2};
    seq++;
    if (!grx_movie_command((uint32_t)cmd, a, sizeof(a))) {
        grx_gsiop_command(seq, cmd, a0, a1, a2);
    }
}

static int32_t st32(const uint8_t *st, int off) {
    int32_t v;
    memcpy(&v, st + off, 4);
    return v;
}
static int16_t st16(const uint8_t *st, int off) {
    int16_t v;
    memcpy(&v, st + off, 2);
    return v;
}

static void test_iop(const char *disc_path, const char *file) {
    GrxDisc *d = grx_disc_open(disc_path);
    GrxDiscEntry e;
    uint8_t *got, *st, header[0x40], buf[0x40 * 2048];
    size_t got_len = 0;
    int rounds = 0, states = 0, last_state = -1, max_ahead = 0;

    if (d == NULL || grx_disc_find(d, file, &e) != 0 || e.is_dir) {
        printf("iop: can't open %s in %s\n", file, disc_path);
        failures++;
        return;
    }
    grx_cdvd_set_disc(d);
    grx_gsiop_boot(header);
    st = grx_gsiop_status();
    got = malloc(pss_len);
    snd_got = malloc(vbs_len + 2048);
    /* MovPlay: 0x100 at mount, open DR (0) and PSS (1), the whole file on
       DR, then 0x300 (0, 1, 2) */
    command(0x100, 0, 0, 0);
    command(0x101, 0, (int32_t)e.lba, (int32_t)((e.size + 2047) / 2048));
    command(0x300, 0, 1, 2);
    /* MovReadLoop: fetch from the PSS ring and release; the audio plays */
    while (rounds++ < 2000000) {
        int ready, state, n;
        grx_movie_poll();
        state = st32(st, 0x2C);
        if (state != last_state) {
            /* 1 (header), 2 (first audio chunk), then 3 and 4 by turns
               (0x300 runs the server at once, so the first seen may be
               any of them) */
            if (state < 1 || state > 4 || (last_state >= 2 && state < 2)) {
                states = -1000;
            }
            states |= 1 << state;
            last_state = state;
        }
        ready = st16(st, 0x80 + 0x20 + 2);
        if (ready > max_ahead) {
            max_ahead = ready;
        }
        if (snd_queued > 0) {
            snd_queued--;
        }
        if (ready <= 0) {
            if (got_len >= (size_t)st32(st, 0x80 + 0x20 + 0x1C) && snd_len >= vbs_len) {
                break;
            }
            continue;
        }
        /* about the movie's rate: 14 video sectors to one audio sector,
           and not past the ring's end (the game's fetch stops there too) */
        n = ready < 14 ? ready : 14;
        if (n > 0x4000 - st16(st, 0x80 + 0x20 + 4)) {
            n = 0x4000 - st16(st, 0x80 + 0x20 + 4);
        }
        grx_gsiop_read((uint32_t)(st32(st, 0x80 + 0x20 + 0x10) + st16(st, 0x80 + 0x20 + 4) * 2048), buf,
                       (uint32_t)n * 2048);
        memcpy(got + got_len, buf, (size_t)n * 2048);
        got_len += (size_t)n * 2048;
        command(0x110, 1, n, 0);
    }
    printf("iop: movieStates seen 0x%X, the PSS ring's total %d; %zu bytes of video fetched, %zu of audio fed; "
           "at most %d sectors ahead\n", states, st32(st, 0x80 + 0x20 + 0x1C), got_len, snd_len, max_ahead);
    CHECK(st32(st, 0x80 + 0x20 + 0x1C) == st32(mov, 0x100),
          "the PSS ring's +0x1C is %d", st32(st, 0x80 + 0x20 + 0x1C));
    CHECK(got_len == pss_len && memcmp(got, pss, pss_len) == 0, "the video stream differs (%zu of %zu bytes)",
          got_len, pss_len);
    CHECK(snd_len == vbs_len && memcmp(snd_got, vbs, vbs_len) == 0, "the audio stream differs (%zu of %zu bytes)",
          snd_len, vbs_len);
    CHECK(states > 0 && (states & 0x18) != 0, "movieState went out of order (0x%X)", states);
    command(0x310, 0, 0, 2);
    CHECK(st32(st, 0x2C) == 0, "movieState %d after 0x310", st32(st, 0x2C));
    free(got);
    free(snd_got);
    grx_disc_close(d);
}
#endif

int main(int argc, char **argv) {
    const char *path = "iso/MOVIE/GRM01.MOV", *yuv = NULL, *outdir = "build/port/movie", *disc = NULL;
    double seconds = 4;
    int every = 15, skip = 0, i;
    char wav[512];

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--seconds") && i + 1 < argc) {
            seconds = atof(argv[++i]);
        } else if (!strcmp(argv[i], "--skip") && i + 1 < argc) {
            skip = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--every") && i + 1 < argc) {
            every = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--yuv") && i + 1 < argc) {
            yuv = argv[++i];
        } else if (!strcmp(argv[i], "--disc") && i + 1 < argc) {
            disc = argv[++i];
        } else if (!strcmp(argv[i], "--frames-out") && i + 1 < argc) {
            outdir = argv[++i];
        } else {
            path = argv[i];
        }
    }
    {
        /* grm01 for iso/MOVIE/GRM01.MOV: the output files' names */
        const char *b = path + strlen(path);
        size_t k;
        while (b > path && b[-1] != '/' && b[-1] != '\\') {
            b--;
        }
        for (k = 0; b[k] && b[k] != '.' && k + 1 < sizeof(base_name); k++) {
            base_name[k] = (char)(b[k] >= 'A' && b[k] <= 'Z' ? b[k] + 32 : b[k]);
        }
        base_name[k] = 0;
    }
    libmpeg_alloc();
    if (load(path) != 0) {
        printf("can't read %s\n", path);
        return 2;
    }
    split();
    demux();
    test_decoder(skip, (int)(seconds * 30), every, outdir, yuv);
    test_whole();
    snprintf(wav, sizeof(wav), "%s/%s.wav", outdir, base_name);
    test_audio(seconds, wav);
    /* the whole movie, to its end */
    test_libmpeg(es, es_len, -1);
#ifdef WITH_IOP
    if (disc != NULL) {
        const char *b = path + strlen(path);
        int up = 0;
        /* the file's path on the disc: MOVIE/GRM01.MOV */
        while (b > path && up < 2) {
            b--;
            if (*b == '/' || *b == '\\') {
                up++;
            }
        }
        test_iop(disc, up == 2 ? b + 1 : b);
    }
#else
    (void)disc;
#endif
    printf(failures ? "%d failures\n" : "all ok (%d failures)\n", failures);
    return failures != 0;
}
