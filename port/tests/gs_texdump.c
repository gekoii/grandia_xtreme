/* Which textures a GS recording's 2D draws use: replays it through the GS
   front end with a backend that, at one vsync, lists each textured
   primitive (its TEX0, its texel and screen rectangles) and writes each
   texture it reads, decoded with the CLUT of the moment, as a PPM. For
   finding where the game's UI art is (port/docs/xbox.md).

   Build as port/tests/gs_replay.c, with port/tests/gs_texdump.c instead.
   Run: gs_texdump.exe RECORDING VSYNC OUTDIR */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_gs.h"
#include "gs_internal.h"
#include "gs_draw.h"

static uint8_t priv[0x2000] __attribute__((aligned(16)));
static uint8_t *buf;
static size_t len, pos;
static int vsyncs, want;
static const char *outdir;
static uint64_t seen[256];
static int nseen;

static uint32_t get32(void) {
    uint32_t v;
    memcpy(&v, buf + pos, 4);
    pos += 4;
    return v;
}

static uint32_t exp16(uint32_t c, uint32_t ta0, uint32_t ta1, uint32_t aem) {
    uint32_t rgb = ((c & 0x1F) << 3) | ((c & 0x3E0) << 6) | ((c & 0x7C00) << 9);
    uint32_t a = (c & 0x8000) ? ta1 : ((!aem || (c & 0x7FFF)) ? ta0 : 0);
    return rgb | (a << 24);
}

/* The texture as 0xAABBGGRR, with the CLUT now */
static void write_tex(uint64_t tex0, const char *name) {
    uint32_t tbp = GS_FIELD(tex0, 0, 14), tbw = GS_FIELD(tex0, 14, 6), psm = GS_FIELD(tex0, 20, 6);
    uint32_t tw = 1u << GS_FIELD(tex0, 26, 4), th = 1u << GS_FIELD(tex0, 30, 4);
    uint32_t cpsm = GS_FIELD(tex0, 51, 4), csa = GS_FIELD(tex0, 56, 5), x, y, pal[256], n, i;
    uint32_t ta0 = GS_FIELD(gs.texa, 0, 8), ta1 = GS_FIELD(gs.texa, 32, 8), aem = GS_FIELD(gs.texa, 15, 1);
    int paletted = psm == GS_PSMT8 || psm == GS_PSMT8H || psm == GS_PSMT4 || psm == GS_PSMT4HL || psm == GS_PSMT4HH;
    char path[512];
    FILE *f;
    if (tw > 1024) tw = 1024;
    if (th > 1024) th = 1024;
    n = (psm == GS_PSMT8 || psm == GS_PSMT8H) ? 256 : 16;
    for (i = 0; paletted && i < n; i++) {
        if ((cpsm & 2) == 0) {
            uint32_t p = ((csa & 15) * 16 + i) & 255;
            pal[i] = gs.clut[p] | ((uint32_t)gs.clut[p + 256] << 16);
        } else {
            pal[i] = exp16(gs.clut[(csa * 16 + i) & 511], ta0, ta1, aem);
        }
    }
    snprintf(path, sizeof(path), "%s/%s.ppm", outdir, name);
    f = fopen(path, "wb");
    if (f == NULL)
        return;
    fprintf(f, "P6\n%u %u\n255\n", tw, th);
    for (y = 0; y < th; y++) {
        for (x = 0; x < tw; x++) {
            uint32_t v = gs_read_pixel(psm, tbp, tbw, x, y), c;
            unsigned char rgb[3];
            if (paletted)
                c = pal[v & (n - 1)];
            else if (psm == GS_PSMCT16 || psm == GS_PSMCT16S)
                c = exp16(v, ta0, ta1, aem);
            else
                c = v;
            /* RGB over black by alpha (0x80 = opaque), so alpha-only art shows */
            {
                uint32_t a = (c >> 24) > 0x80 ? 0x80 : (c >> 24);
                rgb[0] = (unsigned char)((c & 0xFF) * a / 0x80);
                rgb[1] = (unsigned char)(((c >> 8) & 0xFF) * a / 0x80);
                rgb[2] = (unsigned char)(((c >> 16) & 0xFF) * a / 0x80);
            }
            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

static void prim(const GsVertex *v, uint32_t type) {
    static const int nvs[8] = {1, 2, 2, 3, 3, 3, 2, 0};
    uint64_t attr = (gs.prmodecont & 1) ? gs.prim : gs.prmode;
    const GsContext *c = &gs.ctx[GS_FIELD(attr, 9, 1)];
    uint64_t key;
    int nv = nvs[type & 7], i, k;
    int32_t x0, x1, y0, y1, u0, u1, w0, w1;
    char name[64];
    if (vsyncs != want || !GS_FIELD(attr, 4, 1) || nv == 0)
        return;
    x0 = x1 = v[0].x; y0 = y1 = v[0].y;
    u0 = u1 = v[0].u; w0 = w1 = v[0].v;
    for (i = 1; i < nv; i++) {
        if (v[i].x < x0) x0 = v[i].x;
        if (v[i].x > x1) x1 = v[i].x;
        if (v[i].y < y0) y0 = v[i].y;
        if (v[i].y > y1) y1 = v[i].y;
        if (v[i].u < u0) u0 = v[i].u;
        if (v[i].u > u1) u1 = v[i].u;
        if (v[i].v < w0) w0 = v[i].v;
        if (v[i].v > w1) w1 = v[i].v;
    }
    key = c->tex0 & ~(7ull << 61); /* not CLD */
    for (k = 0; k < nseen && seen[k] != key; k++)
        ;
    snprintf(name, sizeof(name), "t%02d_%016llx", k, (unsigned long long)key);
    printf("type %u %s fst %d screen %d,%d-%d,%d uv %d,%d-%d,%d rgba %08x\n", type & 7, name,
           (int)GS_FIELD(attr, 8, 1), x0 / 16, y0 / 16, x1 / 16, y1 / 16, u0 / 16, w0 / 16, u1 / 16, w1 / 16,
           v[0].rgba);
    if (k == nseen && nseen < 256) {
        seen[nseen++] = key;
        write_tex(c->tex0, name);
    }
}

static void nop_rect(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    (void)psm; (void)bp; (void)bw; (void)x; (void)y; (void)w; (void)h;
}
static void nop_pages(const GsPages *p) { (void)p; }
static void nop(void) {}
static void vsync(void) {}
static int display(const uint32_t **pixels, int *width, int *height, int *pitch) {
    (void)pixels; (void)width; (void)height; (void)pitch;
    return 0;
}
static const GsBackend dumper = {prim, nop_rect, nop_pages, nop_pages, nop, vsync, display};

int main(int argc, char **argv) {
    FILE *f;
    int done = 0;
    static uint8_t img[1 << 24];

    if (argc < 4)
        return 2;
    want = atoi(argv[2]);
    outdir = argv[3];
    f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    len = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(len);
    fread(buf, 1, len, f);
    fclose(f);
    grx_gs_set_priv(priv);
    gs_backend = &dumper;
    grx_gs_init();
    pos = 8;
    while (!done && pos < len && vsyncs <= want) {
        int type = buf[pos++];
        switch (type) {
        case 'S': {
            uint32_t n;
            const uint8_t *st;
            get32();
            n = get32();
            st = buf + pos;
            pos += n;
            gs_rec_load(st, buf + pos);
            pos += GS_VRAM_SIZE;
            break;
        }
        case 'G': {
            uint32_t qwc = get32(), path = get32();
            if (qwc)
                grx_gs_gif(buf + pos, qwc, (int)path);
            pos += (size_t)qwc * 16;
            break;
        }
        case 'R': grx_gs_reset(); break;
        case 'X': grx_gs_gif_reset(); break;
        case 'I': {
            uint32_t qwc = get32();
            grx_gs_read_image(img, qwc);
            pos += (size_t)qwc * 16;
            break;
        }
        case 'V': {
            get32();
            memcpy(priv, buf + pos, GS_REC_PRIV);
            pos += GS_REC_PRIV;
            grx_gs_vsync();
            vsyncs++;
            break;
        }
        case 'E': done = 1; break;
        default: fprintf(stderr, "bad record\n"); return 1;
        }
    }
    fprintf(stderr, "%d textures, %d vsyncs\n", nseen, vsyncs);
    return 0;
}
