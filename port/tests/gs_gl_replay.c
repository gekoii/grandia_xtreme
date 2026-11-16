/* Replays a GS recording (GRX_GS_RECORD, port/src/gs/gs_record.c) through
   the GPU renderer (port/src/gs_gl/), in a hidden window: writes each
   vsync's picture as PPM, to compare with the software renderer's
   (port/tests/gs_replay.c -f, then port/tests/gs_compare.py), and times
   the renderer.

   Build (from the repository root; SDL3.dll next to the exe):
     i686-w64-mingw32-clang -std=gnu11 -Wall -O2 -msse2 -mfpmath=sse \
         -Iport/include -Iport/src/gs -Iport/deps/SDL3-3.4.18/i686-w64-mingw32/include \
         port/tests/gs_gl_replay.c port/src/gs/gs_*.c port/src/gs_gl/gsgl.c port/src/gs_gl/gsgl_glsl.c port/src/gl/gl.c \
         -Lport/deps/SDL3-3.4.18/i686-w64-mingw32/lib -lSDL3 -o gs_gl_replay.exe

   Run:
     gs_gl_replay.exe RECORDING [-f ppm-dir] [-e N] [-n repeats] [-s scale] [-sw] [-ws aspect]
   -f writes each vsync's picture (at the internal scale; -e: every Nth), -n replays that
   many times and keeps the fastest, -s sets the internal scale (default
   1, the GS's own resolution), -sw replays through the software GS
   instead (for timing both the same way), -ws 1.778 squeezes the picture
   as for that display aspect, with the scale across to match
   (port/docs/widescreen.md). The times are the GS's work on
   the calling thread plus, at each vsync, waiting for the GPU to finish
   the field (glFinish), so they bound what a field costs. */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "grx_gs.h"
#include "grx_gl.h"
#include "grx_video.h"
#include "gs_internal.h"

static uint8_t priv[0x2000] __attribute__((aligned(16)));
static uint8_t *buf;
static size_t len, pos;
static SDL_Window *win;

/* ---- what the renderer needs from the video layer (grx_video.h) ---- */

void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(1);
}

void *grx_video_gl_create_context(void) {
    SDL_GLContext c;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    win = SDL_CreateWindow("gs_gl_replay", 64, 64, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (win == NULL) {
        grx_log("no window: %s", SDL_GetError());
        return NULL;
    }
    c = SDL_GL_CreateContext(win);
    if (c == NULL) {
        grx_log("no GL 3.3 core context: %s", SDL_GetError());
    }
    return c;
}
int grx_video_gl_make_current(void *ctx) {
    return SDL_GL_MakeCurrent(win, (SDL_GLContext)ctx) ? 0 : -1;
}
void grx_video_gl_source(int on) {
    (void)on;
}
void grx_video_gl_submit(const GrxVideoFrame *frame) {
    (void)frame;
}
int grx_video_gl_texture_free(unsigned texture) {
    (void)texture;
    return 1;
}
void grx_video_window_size(int *w, int *h) {
    *w = *h = 0;
}

/* ---- the replay ---- */

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
    const char *in = NULL, *fdir = NULL;
    int repeats = 1, r, i, sw = 0, every = 1;
    float ws_aspect = 0.0f;
    char scale[16] = "1";
    FILE *f;
    double best = 1e30, *ft = NULL, *best_ft = NULL;
    uint32_t nvs = 0, cap = 0;
    static uint8_t img[1 << 24];

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-f") && i + 1 < argc)
            fdir = argv[++i];
        else if (!strcmp(argv[i], "-n") && i + 1 < argc)
            repeats = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-s") && i + 1 < argc)
            snprintf(scale, sizeof(scale), "%s", argv[++i]);
        else if (!strcmp(argv[i], "-e") && i + 1 < argc)
            every = atoi(argv[++i]) > 0 ? atoi(argv[i]) : 1;
        else if (!strcmp(argv[i], "-sw"))
            sw = 1;
        else if (!strcmp(argv[i], "-ws") && i + 1 < argc)
            ws_aspect = (float)atof(argv[++i]);
        else
            in = argv[i];
    }
    if (!in) {
        fprintf(stderr, "usage: gs_gl_replay RECORDING [-f ppm-dir] [-n repeats] [-s scale] [-sw] [-ws aspect]\n");
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
        fprintf(stderr, "can't read %s\n", in);
        return 1;
    }
    fclose(f);
    if (len < 8 || memcmp(buf, "GSREC001", 8)) {
        fprintf(stderr, "%s: not a GS recording\n", in);
        return 1;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    _putenv_s("GRX_GS_SCALE", scale);
    grx_gs_set_priv(priv);
    grx_gs_init();
    if (ws_aspect > 0.0f)
        gs_ws_set(1, ws_aspect);
    if (!sw && grx_gs_gl_init() != 0) {
        fprintf(stderr, "the GPU renderer didn't start\n");
        return 1;
    }

    for (r = 0; r < repeats; r++) {
        double t_total = 0, t_frame = 0, t0;
        uint32_t nv = 0;
        int done = 0, busy = 0;
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
                t0 = SDL_GetTicksNS() / 1e9;
                grx_gs_gif(buf + pos, qwc, (int)path);
                t_frame += SDL_GetTicksNS() / 1e9 - t0;
                pos += (size_t)qwc * 16;
                busy = 1;
                break;
            }
            case 'R':
                grx_gs_reset();
                break;
            case 'X':
                grx_gs_gif_reset();
                break;
            case 'I': {
                uint32_t qwc = get32();
                t0 = SDL_GetTicksNS() / 1e9;
                grx_gs_read_image(img, qwc);
                t_frame += SDL_GetTicksNS() / 1e9 - t0;
                pos += (size_t)qwc * 16;
                break;
            }
            case 'V': {
                uint32_t vs = get32();
                memcpy(priv, buf + pos, GS_REC_PRIV);
                pos += GS_REC_PRIV;
                if (r == 0 && fdir && vs % (uint32_t)every == 0) {
                    const uint32_t *px = NULL;
                    int w = 0, h = 0, pitch = 0;
                    if (grx_gs_display(&px, &w, &h, &pitch)) {
                        char path[512];
                        FILE *pf;
                        snprintf(path, sizeof(path), "%s/vsync_%06u.ppm", fdir, vs);
                        pf = fopen(path, "wb");
                        if (pf) {
                            int x, y;
                            uint8_t *row = malloc((size_t)w * 3);
                            fprintf(pf, "P6\n%d %d\n255\n", w, h);
                            for (y = 0; y < h; y++) {
                                for (x = 0; x < w; x++) {
                                    uint32_t c = px[y * pitch + x];
                                    row[x * 3] = (uint8_t)(c >> 16);
                                    row[x * 3 + 1] = (uint8_t)(c >> 8);
                                    row[x * 3 + 2] = (uint8_t)c;
                                }
                                fwrite(row, 1, (size_t)w * 3, pf);
                            }
                            free(row);
                            fclose(pf);
                        }
                    }
                }
                t0 = SDL_GetTicksNS() / 1e9;
                grx_gs_vsync();
                if (!sw)
                    glFinish();
                else
                    gs_flush();
                t_frame += SDL_GetTicksNS() / 1e9 - t0;
                if (nv >= cap) {
                    cap = cap ? cap * 2 : 1024;
                    ft = realloc(ft, cap * sizeof(double));
                }
                if (busy)
                    ft[nv++] = t_frame;
                t_total += t_frame;
                t_frame = 0;
                busy = 0;
                break;
            }
            case 'E':
                done = 1;
                break;
            default:
                fprintf(stderr, "bad record '%c' at %u\n", type, (unsigned)(pos - 1));
                return 1;
            }
        }
        nvs = nv;
        printf("run %d: %u frames: %.3f s, %.2f ms a frame\n", r + 1, nv, t_total, nv ? t_total * 1000 / nv : 0);
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
