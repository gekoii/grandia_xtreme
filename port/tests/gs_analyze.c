/* What a GS recording uses: replays it through the GS front end with a
   backend that only counts (port/src/gs/gs_internal.h, GsBackend), and
   prints which features, formats and memory aliasing the game relies on.
   A tool for the GPU renderer's design (port/docs/gpu.md).

   Build as port/tests/gs_replay.c, with port/tests/gs_analyze.c instead.
   Run: gs_analyze.exe RECORDING */
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

static uint32_t get32(void) {
    uint32_t v;
    memcpy(&v, buf + pos, 4);
    pos += 4;
    return v;
}

/* histogram of keys */
typedef struct Hist {
    const char *name;
    uint64_t key[4096];
    uint64_t n[4096];
    int used;
} Hist;
static void hadd(Hist *h, uint64_t k, uint64_t c) {
    int i;
    for (i = 0; i < h->used; i++)
        if (h->key[i] == k) {
            h->n[i] += c;
            return;
        }
    if (h->used < 4096) {
        h->key[h->used] = k;
        h->n[h->used++] = c;
    }
}
static int cmpn(const void *a, const void *b) {
    const uint64_t *x = a, *y = b;
    return x[1] < y[1] ? 1 : x[1] > y[1] ? -1 : 0;
}
static void hprint(Hist *h, const char *fmt) {
    uint64_t (*pairs)[2] = malloc(sizeof(uint64_t) * 2 * (h->used ? h->used : 1));
    int i;
    for (i = 0; i < h->used; i++) {
        pairs[i][0] = h->key[i];
        pairs[i][1] = h->n[i];
    }
    qsort(pairs, (size_t)h->used, sizeof(pairs[0]), cmpn);
    printf("%s (%d kinds):\n", h->name, h->used);
    for (i = 0; i < h->used && i < 40; i++) {
        printf("  %10llu  ", (unsigned long long)pairs[i][1]);
        printf(fmt, (unsigned long long)pairs[i][0]);
        printf("\n");
    }
    free(pairs);
}

static Hist h_prim = {"prim types"}, h_frame = {"frame (fbp fbw psm) by prims"}, h_z = {"z (zbp psm zte ztst zmsk)"},
            h_tex = {"tex psm"}, h_blend = {"blend abcd fix (abe) by prims"}, h_atest = {"alpha test atst aref afail"},
            h_misc = {"misc: fba date datm fge dthe colclamp=0 pabe fbmsk!=0 scanmsk"}, h_fbmsk = {"fbmsk"},
            h_tfx = {"tfx tcc"}, h_clamp = {"clamp wms wmt"}, h_filt = {"filter mag min lcm mxl"},
            h_fst = {"fst"}, h_upl = {"uploads psm (bytes)"}, h_rtt = {"render-to-texture: texpsm framepsm tbw fbw offset-pages"},
            h_uprt = {"uploads into rendered pages: psm full?"}, h_clutrt = {"CLUT loads from rendered pages"},
            h_alpha_over = {"blend using As with vertex alpha > 0x80 (abcd)"}, h_texsize = {"tex tw th"},
            h_zrt = {"frame draws into Z-rendered pages / z draws into frame-rendered"},
            h_l2l = {"local-to-local spsm dpsm w h"}, h_ctx = {"draws per frame (state changes)"},
            h_mip = {"mxl"}, h_texa = {"texa"};

static uint32_t page_key[512]; /* 0: memory; else key of the last draw writing it (bit 31: z) */
static uint64_t prims_frame, states_frame;
static int nframes;
static GsDraw D;
static int have_state;

static void pages_of(GsPages *s, uint32_t psm, uint32_t bp, uint32_t bw, int x0, int y0, int x1, int y1) {
    memset(s, 0, sizeof(*s));
    if (x1 > x0 && y1 > y0)
        gs_pages_rect(s, psm, bp, bw, (uint32_t)x0, (uint32_t)y0, (uint32_t)(x1 - x0), (uint32_t)(y1 - y0));
}

static void prim(const GsVertex *v, uint32_t type) {
    static const int nvs[8] = {1, 2, 2, 3, 3, 3, 2, 0};
    int nv = nvs[type & 7], i, x0, y0, x1, y1;
    uint32_t fpsm, attrx;
    uint64_t attr = (gs.prmodecont & 1) ? gs.prim : gs.prmode;
    const GsContext *c = &gs.ctx[GS_FIELD(attr, 9, 1)];
    GsPages fp, zp;

    if (gs.dirty || !have_state) {
        gs_setup(&D);
        gs.dirty = 0;
        have_state = 1;
        states_frame++;
    }
    prims_frame++;
    hadd(&h_prim, type, 1);
    fpsm = GS_FIELD(c->frame, 24, 6);
    hadd(&h_frame, (uint64_t)D.fbp << 32 | D.fbw << 16 | fpsm, 1);
    hadd(&h_z, (uint64_t)D.zbp << 32 | (0x30 | GS_FIELD(c->zbuf, 24, 4)) << 16 | D.zte << 8 | D.ztst << 4 | !D.zwrite,
         1);
    hadd(&h_blend, D.abe ? (uint64_t)(D.ba << 12 | D.bb << 8 | D.bc << 4 | D.bd) | (uint64_t)D.fix << 16 : 0xFFFFFF, 1);
    hadd(&h_atest, D.ate ? (uint64_t)(D.atst << 16 | D.aref << 4 | D.afail) : 0xFFFFFF, 1);
    hadd(&h_misc,
         (uint64_t)(D.fba << 28 | D.date << 24 | D.datm << 20 | D.fge << 16 | D.dthe << 12 | (!D.colclamp) << 8 |
                    D.pabe << 4 | (D.fbmsk != (D.f24 ? 0xFF000000u : 0)) << 2 | D.scanmsk),
         1);
    hadd(&h_fbmsk, D.fbmsk, 1);
    attrx = 0;
    for (i = 0; i < nv; i++)
        if ((v[i].rgba >> 24) > 0x80)
            attrx = 1;
    if (attrx && D.abe && (D.bc == 0))
        hadd(&h_alpha_over, (uint64_t)(D.ba << 12 | D.bb << 8 | D.bc << 4 | D.bd), 1);
    if (D.tme) {
        hadd(&h_tex, D.t.psm, 1);
        hadd(&h_tfx, D.tfx << 4 | D.tcc, 1);
        hadd(&h_clamp, D.t.wms << 4 | D.t.wmt, 1);
        hadd(&h_filt, D.t.mag_linear << 12 | D.t.min_linear << 8 | D.t.lcm << 4 | GS_FIELD(c->tex1, 2, 3), 1);
        hadd(&h_fst, D.fst, 1);
        hadd(&h_texsize, D.t.tw << 8 | D.t.th, 1);
        hadd(&h_texa, gs.texa, 1);
        if (D.t.psm == 0 && getenv("AN_RTT") && nframes < 40)
            printf("rtt f%d: fbp %x tbp %x tbw %u tw %u th %u type %u abe %d %x%x%x%x fix %x atst %d afail %d fst %d mag %d "
                   "v0 %d,%d uv %d,%d st %g,%g q %g rgba %08x  v1 %d,%d uv %d,%d st %g,%g q %g zte %d ztst %d zw %d\n",
                   nframes, D.fbp, D.t.tbp, D.t.tbw, D.t.tw, D.t.th, type, D.abe, D.ba, D.bb, D.bc, D.bd, D.fix,
                   D.atst, D.afail, D.fst, D.t.mag_linear, v[0].x, v[0].y, v[0].u, v[0].v, v[0].s, v[0].t, v[0].q,
                   v[0].rgba, v[1].x, v[1].y, v[1].u, v[1].v, v[1].s, v[1].t, v[1].q, D.zte, D.ztst, D.zwrite);
        /* render to texture: texture pages written by draws */
        {
            int k;
            for (k = 0; k < 512; k++)
                if ((D.rtex.w[k >> 5] >> (k & 31)) & 1 && page_key[k]) {
                    uint32_t pk = page_key[k];
                    uint32_t ppsm = pk & 63, pbw = (pk >> 6) & 63, pbp = (pk >> 12) & 0x3FFF;
                    hadd(&h_rtt,
                         (uint64_t)D.t.psm << 40 | (uint64_t)ppsm << 32 | D.t.tbw << 24 | pbw << 16 |
                             (((int)D.t.tbp - (int)pbp) & 0xFFFF),
                         1);
                    break;
                }
        }
    }
    /* the frame and Z rectangles written */
    x0 = y0 = 1 << 30;
    x1 = y1 = -(1 << 30);
    for (i = 0; i < nv; i++) {
        if (v[i].x >> 4 < x0) x0 = v[i].x >> 4;
        if (v[i].y >> 4 < y0) y0 = v[i].y >> 4;
        if ((v[i].x + 15) >> 4 > x1) x1 = (v[i].x + 15) >> 4;
        if ((v[i].y + 15) >> 4 > y1) y1 = (v[i].y + 15) >> 4;
    }
    if (x0 < D.scx0) x0 = D.scx0;
    if (y0 < D.scy0) y0 = D.scy0;
    if (x1 > D.scx1 + 1) x1 = D.scx1 + 1;
    if (y1 > D.scy1 + 1) y1 = D.scy1 + 1;
    if (x0 >= x1 || y0 >= y1 || !D.fvalid)
        return;
    pages_of(&fp, fpsm, D.fbp, D.fbw, x0, y0, x1, y1);
    pages_of(&zp, 0x30 | GS_FIELD(c->zbuf, 24, 4), D.zbp, D.fbw, x0, y0, x1, y1);
    for (i = 0; i < 512; i++) {
        if ((fp.w[i >> 5] >> (i & 31)) & 1) {
            if (page_key[i] & 0x80000000u)
                hadd(&h_zrt, 1, 1);
            page_key[i] = fpsm | D.fbw << 6 | D.fbp << 12;
        }
        if (D.zwrite && ((zp.w[i >> 5] >> (i & 31)) & 1)) {
            if (page_key[i] && !(page_key[i] & 0x80000000u))
                hadd(&h_zrt, 2, 1);
            page_key[i] = 0x80000000u | (0x30 | GS_FIELD(c->zbuf, 24, 4)) | D.fbw << 6 | (D.zbp & 0x3FFF) << 12;
        }
    }
}

static uint32_t up_psm, up_full;
static void writing(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    GsPages p;
    int i, rendered = 0;
    memset(&p, 0, sizeof(p));
    gs_pages_rect(&p, psm, bp, bw, x, y, w, h);
    for (i = 0; i < 512; i++)
        if (((p.w[i >> 5] >> (i & 31)) & 1) && page_key[i]) {
            rendered = 1;
            page_key[i] = 0;
        }
    hadd(&h_upl, psm, (uint64_t)w * h);
    if (psm == 0 && w >= 256 && getenv("AN_RTT") && nframes < 40)
        printf("upload f%d: bp %x bw %u %u,%u %ux%u rendered %d\n", nframes, bp, bw, x, y, w, h, rendered);
    if (rendered)
        hadd(&h_uprt, (uint64_t)psm << 32 | w << 16 | h, 1);
    up_psm = psm;
    up_full = 0;
}
static void wrote(const GsPages *p) {
    (void)p;
}
static int in_clut;
static void reading(const GsPages *p) {
    int i;
    for (i = 0; i < 512; i++)
        if (((p->w[i >> 5] >> (i & 31)) & 1) && page_key[i]) {
            hadd(&h_clutrt, page_key[i], 1);
            break;
        }
    (void)in_clut;
}
static void loaded(void) {
    memset(page_key, 0, sizeof(page_key));
}
static uint64_t frames_with_draws;
static void vsync(void) {
    if (prims_frame) {
        hadd(&h_ctx, states_frame / 50 * 50, 1);
        frames_with_draws++;
    }
    nframes++;
    prims_frame = states_frame = 0;
}
static int display(const uint32_t **pixels, int *width, int *height, int *pitch) {
    (void)pixels, (void)width, (void)height, (void)pitch;
    return 0;
}
static const GsBackend analyze = {prim, writing, wrote, reading, loaded, vsync, display};

int main(int argc, char **argv) {
    FILE *f;
    int done = 0;
    static uint8_t img[1 << 24];
    uint64_t nl2l = 0, nread = 0;

    if (argc < 2)
        return 2;
    f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    len = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(len);
    fread(buf, 1, len, f);
    fclose(f);
    grx_gs_set_priv(priv);
    gs_backend = &analyze;
    grx_gs_init();
    pos = 8;
    while (!done && pos < len) {
        int type = buf[pos++];
        switch (type) {
        case 'S': {
            uint32_t vs = get32(), n = get32();
            const uint8_t *st = buf + pos;
            (void)vs;
            pos += n;
            gs_rec_load(st, buf + pos);
            pos += GS_VRAM_SIZE;
            break;
        }
        case 'G': {
            uint32_t qwc = get32(), path = get32();
            uint64_t before = gs.trxdir;
            (void)before;
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
            nread++;
            pos += (size_t)qwc * 16;
            break;
        }
        case 'V': {
            uint32_t vs = get32();
            (void)vs;
            memcpy(priv, buf + pos, GS_REC_PRIV);
            pos += GS_REC_PRIV;
            grx_gs_vsync();
            break;
        }
        case 'E': done = 1; break;
        default: fprintf(stderr, "bad record\n"); return 1;
        }
    }
    (void)nl2l;
    printf("%d vsyncs, %llu with draws, %llu read-backs\n", nframes, (unsigned long long)frames_with_draws,
           (unsigned long long)nread);
    hprint(&h_ctx, "%llu states/frame (bucket)");
    hprint(&h_prim, "type %llu");
    hprint(&h_frame, "%016llx");
    hprint(&h_z, "%016llx");
    hprint(&h_blend, "%06llx");
    hprint(&h_alpha_over, "%04llx");
    hprint(&h_atest, "%06llx");
    hprint(&h_misc, "%08llx");
    hprint(&h_fbmsk, "%08llx");
    hprint(&h_tex, "psm %02llx");
    hprint(&h_texsize, "%04llx");
    hprint(&h_texa, "%016llx");
    hprint(&h_tfx, "%02llx");
    hprint(&h_clamp, "%02llx");
    hprint(&h_filt, "%05llx");
    hprint(&h_fst, "%llu");
    hprint(&h_upl, "psm %02llx");
    hprint(&h_uprt, "%016llx");
    hprint(&h_rtt, "%016llx");
    hprint(&h_clutrt, "%08llx");
    hprint(&h_zrt, "%llu");
    (void)h_l2l;
    (void)h_mip;
    return 0;
}
