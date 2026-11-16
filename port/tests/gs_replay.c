/* Replays a GS recording (GRX_GS_RECORD, port/src/gs/gs_record.c): feeds
   the software GS the same calls the game made, and for every vsync
   prints a hash of the displayed picture and of the whole 4 MB of local
   memory, so two renderers can be compared line by line, and times the
   GS's work per vsync.

   Build (from the repository root; any output directory; SDL3.dll next to
   the exe):
     i686-w64-mingw32-clang -std=gnu11 -Wall -O1 -msse2 -mfpmath=sse \
         -Iport/include -Iport/src/gs -Iport/deps/SDL3-3.4.18/i686-w64-mingw32/include \
         port/tests/gs_replay.c port/src/gs/gs_*.c \
         -Lport/deps/SDL3-3.4.18/i686-w64-mingw32/lib -lSDL3 -o gs_replay.exe

   Run:
     gs_replay.exe RECORDING [-o hashes.txt] [-n repeats] [-f ppm-dir]
                   [-novram] [-ws aspect]
   -o writes the per-vsync hashes (first run only), -n replays the whole
   recording that many times and keeps the fastest time, -f writes each
   vsync's picture as PPM, -novram skips the local-memory hash, -ws 1.778
   squeezes the picture as for that display aspect (port/docs/
   widescreen.md; the recording's culling stays 4:3). */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grx_gs.h"
#include "gs_internal.h"

static uint8_t priv[0x2000] __attribute__((aligned(16)));

static uint8_t *buf;
static size_t len, pos;

static double now_s(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

static uint64_t fnv(const void *p, size_t n, uint64_t h) {
    const uint8_t *b = p;
    const uint64_t *w = p;
    size_t i;
    /* word at a time: fast enough for 4 MB a vsync */
    for (i = 0; i + 8 <= n; i += 8)
        h = (h ^ w[i / 8]) * 0x100000001B3ull;
    for (; i < n; i++)
        h = (h ^ b[i]) * 0x100000001B3ull;
    return h;
}

static uint32_t get32(void) {
    uint32_t v;
    if (pos + 4 > len) {
        fprintf(stderr, "truncated recording\n");
        exit(1);
    }
    memcpy(&v, buf + pos, 4);
    pos += 4;
    return v;
}

static int cmp_d(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return x < y ? -1 : x > y;
}

int main(int argc, char **argv) {
    const char *in = NULL, *out = NULL, *fdir = NULL;
    int repeats = 1, vram_hash = 1, r, i;
    float ws_aspect = 0.0f;
    FILE *f, *o = NULL;
    double best = 1e30, *ft = NULL, *best_ft = NULL;
    uint32_t nvs = 0, cap = 0;
    static uint8_t img[1 << 24];

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o") && i + 1 < argc)
            out = argv[++i];
        else if (!strcmp(argv[i], "-n") && i + 1 < argc)
            repeats = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-f") && i + 1 < argc)
            fdir = argv[++i];
        else if (!strcmp(argv[i], "-novram"))
            vram_hash = 0;
        else if (!strcmp(argv[i], "-ws") && i + 1 < argc)
            ws_aspect = (float)atof(argv[++i]);
        else
            in = argv[i];
    }
    if (!in) {
        fprintf(stderr, "usage: gs_replay RECORDING [-o hashes] [-n repeats] [-f ppm-dir] [-novram] [-ws aspect]\n");
        return 2;
    }
    f = fopen(in, "rb");
    if (!f) {
        perror(in);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    len = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(len);
    if (!buf || fread(buf, 1, len, f) != len) {
        fprintf(stderr, "can't read %s (%zu bytes)\n", in, len);
        return 1;
    }
    fclose(f);
    if (len < 8 || memcmp(buf, "GSREC001", 8)) {
        fprintf(stderr, "%s: not a GS recording\n", in);
        return 1;
    }
    grx_gs_set_priv(priv);
    grx_gs_init();
    if (ws_aspect > 0.0f)
        gs_ws_set(1, ws_aspect);

    for (r = 0; r < repeats; r++) {
        double t_total = 0, t0, t;
        uint32_t nv = 0, nimg = 0, bad_img = 0, ngif = 0, nidle = 0;
        uint64_t gif_bytes = 0;
        double t_frame = 0;
        int done = 0, busy = 0;

        if (r == 0 && out) {
            o = fopen(out, "w");
            if (!o) {
                perror(out);
                return 1;
            }
        }
        pos = 8;
        while (!done && pos < len) {
            int type = buf[pos++];
            switch (type) {
            case 'S': {
                uint32_t vs = get32(), n = get32();
                const uint8_t *st = buf + pos;
                pos += n;
                gs_rec_load(st, buf + pos);
                pos += GS_VRAM_SIZE;
                if (r == 0)
                    printf("snapshot at vsync %u\n", vs);
                break;
            }
            case 'G': {
                uint32_t qwc = get32(), path = get32();
                t0 = now_s();
                grx_gs_gif(buf + pos, qwc, (int)path);
                t = now_s() - t0;
                t_frame += t;
                pos += (size_t)qwc * 16;
                ngif++;
                busy = 1;
                gif_bytes += (uint64_t)qwc * 16;
                break;
            }
            case 'R':
                t0 = now_s();
                grx_gs_reset();
                t_frame += now_s() - t0;
                break;
            case 'X':
                grx_gs_gif_reset();
                break;
            case 'I': {
                uint32_t qwc = get32();
                if ((size_t)qwc * 16 > sizeof(img)) {
                    fprintf(stderr, "read_image too big\n");
                    return 1;
                }
                t0 = now_s();
                grx_gs_read_image(img, qwc);
                t_frame += now_s() - t0;
                if (memcmp(img, buf + pos, (size_t)qwc * 16))
                    bad_img++;
                if (o)
                    fprintf(o, "I %u %016llx\n", nimg, (unsigned long long)fnv(img, (size_t)qwc * 16, 0xCBF29CE484222325ull));
                nimg++;
                pos += (size_t)qwc * 16;
                break;
            }
            case 'V': {
                uint32_t vs = get32();
                const uint32_t *px = NULL;
                int w = 0, h = 0, pitch = 0, on;
                uint64_t dh = 0, vh = 0;
                memcpy(priv, buf + pos, GS_REC_PRIV);
                pos += GS_REC_PRIV;
                t0 = now_s();
                on = grx_gs_display(&px, &w, &h, &pitch);
                t_frame += now_s() - t0;
                if (on) {
                    int y;
                    dh = 0xCBF29CE484222325ull ^ (uint64_t)w ^ ((uint64_t)h << 16);
                    for (y = 0; y < h; y++)
                        dh = fnv(px + (size_t)y * pitch, (size_t)w * 4, dh);
                }
                if (r == 0 && (o || fdir)) {
                    if (vram_hash) {
                        gs_flush();
                        vh = fnv(gs_vram, GS_VRAM_SIZE, 0xCBF29CE484222325ull);
                    }
                    if (o)
                        fprintf(o, "V %u %d %dx%d %016llx %016llx\n", vs, on, w, h, (unsigned long long)dh,
                                (unsigned long long)vh);
                    if (fdir && on) {
                        char path[512];
                        FILE *pf;
                        snprintf(path, sizeof(path), "%s/vsync_%06u.ppm", fdir, vs);
                        pf = fopen(path, "wb");
                        if (pf) {
                            int x, y;
                            fprintf(pf, "P6\n%d %d\n255\n", w, h);
                            for (y = 0; y < h; y++)
                                for (x = 0; x < w; x++) {
                                    uint32_t c = px[y * pitch + x];
                                    uint8_t rgb[3] = {(uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c};
                                    fwrite(rgb, 1, 3, pf);
                                }
                            fclose(pf);
                        }
                    }
                }
                grx_gs_vsync();
                if (nv >= cap) {
                    cap = cap ? cap * 2 : 1024;
                    ft = realloc(ft, cap * sizeof(double));
                }
                /* vsyncs after which the game drew nothing (catching up
                   on fields a slow frame missed) don't count as frames */
                if (busy)
                    ft[nv++] = t_frame;
                else
                    nidle++;
                t_total += t_frame;
                t_frame = 0;
                busy = 0;
                break;
            }
            case 'E':
                done = 1;
                break;
            default:
                fprintf(stderr, "bad record '%c' at %zu\n", type, pos - 1);
                return 1;
            }
        }
        /* what the GS still has queued counts too */
        t0 = now_s();
        gs_flush();
        t_total += now_s() - t0;
        if (o) {
            fclose(o);
            o = NULL;
        }
        nvs = nv;
        printf("run %d: %u frames (+%u idle vsyncs), %u GIF calls (%.1f MB), %u reads (%u differ from the "
               "recording): GS %.3f s, %.2f ms a frame\n",
               r + 1, nv, nidle, ngif, gif_bytes / 1048576.0, nimg, bad_img, t_total, nv ? t_total * 1000 / nv : 0);
        if (t_total < best) {
            best = t_total;
            free(best_ft);
            best_ft = malloc((nv ? nv : 1) * sizeof(double));
            memcpy(best_ft, ft, nv * sizeof(double));
        }
    }
    if (nvs) {
        qsort(best_ft, nvs, sizeof(double), cmp_d);
        printf("best: %.3f s; per frame: mean %.2f ms, median %.2f, p90 %.2f, p99 %.2f, max %.2f\n", best,
               best * 1000 / nvs, best_ft[nvs / 2] * 1000, best_ft[nvs * 9 / 10] * 1000,
               best_ft[nvs * 99 / 100] * 1000, best_ft[nvs - 1] * 1000);
    }
    return 0;
}
