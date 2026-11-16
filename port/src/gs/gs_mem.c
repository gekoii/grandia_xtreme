/* The GS's local memory: the swizzles of every pixel format, image
   transfers (host to local, local to host, local to local) and CLUT
   loading. */
#include "gs_internal.h"
#include "gs_draw.h"

uint8_t gs_vram[GS_VRAM_SIZE] __attribute__((aligned(64)));

uint16_t gs_pg32[2][32][64];
uint16_t gs_pg16[4][64][64];
uint16_t gs_pg8[64][128];
uint16_t gs_pg4[128][128];
GsPsmInfo gs_psm[64];

/* Block order within a page */
static const uint8_t bt32[4][8] = {
    {0, 1, 4, 5, 16, 17, 20, 21},
    {2, 3, 6, 7, 18, 19, 22, 23},
    {8, 9, 12, 13, 24, 25, 28, 29},
    {10, 11, 14, 15, 26, 27, 30, 31},
};
static const uint8_t bt16[8][4] = {
    {0, 2, 8, 10}, {1, 3, 9, 11}, {4, 6, 12, 14}, {5, 7, 13, 15},
    {16, 18, 24, 26}, {17, 19, 25, 27}, {20, 22, 28, 30}, {21, 23, 29, 31},
};
static const uint8_t bt16s[8][4] = {
    {0, 2, 16, 18}, {1, 3, 17, 19}, {8, 10, 24, 26}, {9, 11, 25, 27},
    {4, 6, 20, 22}, {5, 7, 21, 23}, {12, 14, 28, 30}, {13, 15, 29, 31},
};

/* Word of a 32-bit pixel within its block: four columns of 8x2 pixels,
   pixels interleaved in pairs */
static uint32_t col32(uint32_t x, uint32_t y) {
    return (y >> 1) * 16 + ((x & 1) | ((y & 1) << 1) | (((x >> 1) & 1) << 2) | (((x >> 2) & 1) << 3));
}
/* 16-bit: a column is 16x2; pixels 0-7 of a row are the low halves of
   the words a 32-bit row has, 8-15 the high halves */
static uint32_t col16(uint32_t x, uint32_t y) {
    return col32(x & 7, y) * 2 + ((x >> 3) & 1);
}
/* 8-bit (16x16 block, columns of 16x4) and 4-bit (32x16, columns of
   32x4): each column is one 32-bit column's 16 words. Rows 0-1 and 2-3
   of a column share words (rows 2-3 in the odd bytes/nibbles), and in
   every other column-and-row-pair the two halves of the 8 words swap. */
static uint32_t col8(uint32_t x, uint32_t y) {
    uint32_t c = y >> 2, r = y & 3;
    uint32_t swap = (c & 1) ^ (r >> 1);
    uint32_t word = col32((x & 7) ^ (swap ? 4 : 0), c * 2 + (r & 1));
    return word * 4 + ((x >> 3) & 1) * 2 + (r >> 1);
}
static uint32_t col4(uint32_t x, uint32_t y) {
    uint32_t c = y >> 2, r = y & 3;
    uint32_t swap = (c & 1) ^ (r >> 1);
    uint32_t word = col32((x & 7) ^ (swap ? 4 : 0), c * 2 + (r & 1));
    return word * 8 + ((x >> 3) & 3) * 2 + (r >> 1);
}

static void psm_set(uint32_t psm, int layout, int kind) {
    gs_psm[psm].layout = (uint8_t)layout;
    gs_psm[psm].kind = (uint8_t)kind;
    gs_psm[psm].valid = 1;
}

void gs_mem_init(void) {
    uint32_t x, y, z, t;
    static int done;

    if (done)
        return;
    done = 1;
    for (z = 0; z < 2; z++)
        for (y = 0; y < 32; y++)
            for (x = 0; x < 64; x++)
                gs_pg32[z][y][x] = (uint16_t)(((bt32[y >> 3][x >> 3] ^ (z ? 0x18 : 0)) << 6) + col32(x & 7, y & 7));
    for (t = 0; t < 4; t++)
        for (y = 0; y < 64; y++)
            for (x = 0; x < 64; x++) {
                uint32_t b = (t & 1) ? bt16s[y >> 3][x >> 4] : bt16[y >> 3][x >> 4];
                if (t & 2)
                    b ^= 0x18;
                gs_pg16[t][y][x] = (uint16_t)((b << 7) + col16(x & 15, y & 7));
            }
    for (y = 0; y < 64; y++)
        for (x = 0; x < 128; x++)
            gs_pg8[y][x] = (uint16_t)((bt32[y >> 4][x >> 4] << 8) + col8(x & 15, y & 15));
    for (y = 0; y < 128; y++)
        for (x = 0; x < 128; x++)
            gs_pg4[y][x] = (uint16_t)((bt16[y >> 4][x >> 5] << 9) + col4(x & 31, y & 15));

    /* Undefined PSMs read and write as PSMCT32 (valid stays 0) */
    for (x = 0; x < 64; x++) {
        gs_psm[x].layout = GS_L32;
        gs_psm[x].kind = GS_K32;
    }
    psm_set(GS_PSMCT32, GS_L32, GS_K32);
    psm_set(GS_PSMCT24, GS_L32, GS_K24);
    psm_set(GS_PSMCT16, GS_L16, GS_K16);
    psm_set(GS_PSMCT16S, GS_L16S, GS_K16);
    psm_set(GS_PSMT8, GS_L8, GS_K8);
    psm_set(GS_PSMT4, GS_L4, GS_K4);
    psm_set(GS_PSMT8H, GS_L32, GS_K8H);
    psm_set(GS_PSMT4HL, GS_L32, GS_K4HL);
    psm_set(GS_PSMT4HH, GS_L32, GS_K4HH);
    psm_set(GS_PSMZ32, GS_L32Z, GS_K32);
    psm_set(GS_PSMZ24, GS_L32Z, GS_K24);
    psm_set(GS_PSMZ16, GS_L16Z, GS_K16);
    psm_set(GS_PSMZ16S, GS_L16SZ, GS_K16);
}

/* The address of a pixel in its format's units */
static inline uint32_t psm_addr(const GsPsmInfo *pi, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    switch (pi->layout) {
    case GS_L32: return gs_addr32(0, bp, bw, x, y);
    case GS_L32Z: return gs_addr32(1, bp, bw, x, y);
    case GS_L16: return gs_addr16(0, bp, bw, x, y);
    case GS_L16S: return gs_addr16(1, bp, bw, x, y);
    case GS_L16Z: return gs_addr16(2, bp, bw, x, y);
    case GS_L16SZ: return gs_addr16(3, bp, bw, x, y);
    case GS_L8: return gs_addr8(bp, bw, x, y);
    default: return gs_addr4(bp, bw, x, y);
    }
}

uint32_t gs_read_pixel(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    const GsPsmInfo *pi = &gs_psm[psm & 63];
    uint32_t a = psm_addr(pi, bp, bw, x, y);

    switch (pi->kind) {
    case GS_K32: return GS_VRAM32[a];
    case GS_K24: return GS_VRAM32[a] & 0xFFFFFF;
    case GS_K16: return GS_VRAM16[a];
    case GS_K8: return gs_vram[a];
    case GS_K4: return (gs_vram[a >> 1] >> ((a & 1) * 4)) & 15;
    case GS_K8H: return GS_VRAM32[a] >> 24;
    case GS_K4HL: return (GS_VRAM32[a] >> 24) & 15;
    default: return GS_VRAM32[a] >> 28;
    }
}

void gs_write_pixel(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t v) {
    const GsPsmInfo *pi = &gs_psm[psm & 63];
    uint32_t a = psm_addr(pi, bp, bw, x, y);
    uint8_t *b;

    switch (pi->kind) {
    case GS_K32: GS_VRAM32[a] = v; break;
    case GS_K24: GS_VRAM32[a] = (GS_VRAM32[a] & 0xFF000000u) | (v & 0xFFFFFF); break;
    case GS_K16: GS_VRAM16[a] = (uint16_t)v; break;
    case GS_K8: gs_vram[a] = (uint8_t)v; break;
    case GS_K4:
        b = &gs_vram[a >> 1];
        if (a & 1)
            *b = (uint8_t)((*b & 0x0F) | (v << 4));
        else
            *b = (uint8_t)((*b & 0xF0) | (v & 15));
        break;
    case GS_K8H: gs_vram[a * 4 + 3] = (uint8_t)v; break;
    case GS_K4HL: b = &gs_vram[a * 4 + 3]; *b = (uint8_t)((*b & 0xF0) | (v & 15)); break;
    default: b = &gs_vram[a * 4 + 3]; *b = (uint8_t)((*b & 0x0F) | (v << 4)); break;
    }
}

/* ---- transfers ---- */

/* Bits per pixel as a transfer sends them */
static uint32_t trx_bits(uint32_t psm) {
    switch (gs_psm[psm & 63].kind) {
    case GS_K32: return 32;
    case GS_K24: return 24;
    case GS_K16: return 16;
    case GS_K8: case GS_K8H: return 8;
    default: return 4;
    }
}

static void trx_setup(GsTransfer *t, uint32_t bp, uint32_t bw, uint32_t psm, uint32_t sx, uint32_t sy) {
    t->bp = bp;
    t->bw = bw;
    t->psm = psm;
    t->sx = sx;
    t->x = sx;
    t->y = sy;
    t->w = GS_FIELD(gs.trxreg, 0, 12);
    t->h = GS_FIELD(gs.trxreg, 32, 12);
    t->left = t->w * t->h;
    t->npend = 0;
    t->active = t->left != 0;
}

static inline void trx_next(GsTransfer *t) {
    if (++t->x - t->sx >= t->w) {
        t->x = t->sx;
        t->y++;
    }
    if (--t->left == 0)
        t->active = 0;
}

static void local_to_local(void) {
    uint32_t sbp = GS_FIELD(gs.bitbltbuf, 0, 14), sbw = GS_FIELD(gs.bitbltbuf, 16, 6);
    uint32_t spsm = GS_FIELD(gs.bitbltbuf, 24, 6);
    uint32_t dbp = GS_FIELD(gs.bitbltbuf, 32, 14), dbw = GS_FIELD(gs.bitbltbuf, 48, 6);
    uint32_t dpsm = GS_FIELD(gs.bitbltbuf, 56, 6);
    uint32_t sx = GS_FIELD(gs.trxpos, 0, 11), sy = GS_FIELD(gs.trxpos, 16, 11);
    uint32_t dx = GS_FIELD(gs.trxpos, 32, 11), dy = GS_FIELD(gs.trxpos, 48, 11);
    uint32_t dir = GS_FIELD(gs.trxpos, 59, 2);
    uint32_t w = GS_FIELD(gs.trxreg, 0, 12), h = GS_FIELD(gs.trxreg, 32, 12);
    uint32_t i, j;

    /* DIR: 0 from the upper left, 1 lower left, 2 upper right, 3 lower right */
    for (j = 0; j < h; j++) {
        uint32_t yy = (dir & 1) ? h - 1 - j : j;
        for (i = 0; i < w; i++) {
            uint32_t xx = (dir & 2) ? w - 1 - i : i;
            uint32_t v = gs_read_pixel(spsm, sbp, sbw, sx + xx, sy + yy);
            gs_write_pixel(dpsm, dbp, dbw, dx + xx, dy + yy, v);
        }
    }
}

/* The pages the host-to-local transfer writes */
static GsPages up_pages;

void gs_trx_start(void) {
    uint32_t xdir = GS_FIELD(gs.trxdir, 0, 2);
    GsTransfer *t = &gs.up;

    switch (xdir) {
    case 0:
        trx_setup(t, GS_FIELD(gs.bitbltbuf, 32, 14), GS_FIELD(gs.bitbltbuf, 48, 6),
                  GS_FIELD(gs.bitbltbuf, 56, 6), GS_FIELD(gs.trxpos, 32, 11), GS_FIELD(gs.trxpos, 48, 11));
        memset(&up_pages, 0, sizeof(up_pages));
        gs_pages_rect(&up_pages, t->psm, t->bp, t->bw, t->sx, t->y, t->w, t->h);
        gs_ws_uploaded(&up_pages);
        if (gs_backend && t->active)
            gs_backend->writing(t->psm, t->bp, t->bw, t->sx, t->y, t->w, t->h);
        break;
    case 1:
        trx_setup(&gs.down, GS_FIELD(gs.bitbltbuf, 0, 14), GS_FIELD(gs.bitbltbuf, 16, 6),
                  GS_FIELD(gs.bitbltbuf, 24, 6), GS_FIELD(gs.trxpos, 0, 11), GS_FIELD(gs.trxpos, 16, 11));
        break;
    case 2:
        gs_flush();
        if (gs_backend) {
            GsPages src, dst;
            uint32_t w = GS_FIELD(gs.trxreg, 0, 12), h = GS_FIELD(gs.trxreg, 32, 12);
            memset(&src, 0, sizeof(src));
            memset(&dst, 0, sizeof(dst));
            gs_pages_rect(&src, GS_FIELD(gs.bitbltbuf, 24, 6), GS_FIELD(gs.bitbltbuf, 0, 14),
                          GS_FIELD(gs.bitbltbuf, 16, 6), GS_FIELD(gs.trxpos, 0, 11), GS_FIELD(gs.trxpos, 16, 11), w,
                          h);
            gs_pages_rect(&dst, GS_FIELD(gs.bitbltbuf, 56, 6), GS_FIELD(gs.bitbltbuf, 32, 14),
                          GS_FIELD(gs.bitbltbuf, 48, 6), GS_FIELD(gs.trxpos, 32, 11), GS_FIELD(gs.trxpos, 48, 11), w,
                          h);
            gs_backend->reading(&src);
            gs_backend->writing(GS_FIELD(gs.bitbltbuf, 56, 6), GS_FIELD(gs.bitbltbuf, 32, 14),
                                GS_FIELD(gs.bitbltbuf, 48, 6), GS_FIELD(gs.trxpos, 32, 11),
                                GS_FIELD(gs.trxpos, 48, 11), w, h);
            local_to_local();
            gs_backend->wrote(&dst);
            break;
        }
        local_to_local();
        break;
    default:
        gs.up.active = 0;
        gs.down.active = 0;
        break;
    }
}

/* A run of pixels along a row of an image transfer into one of the common
   formats: `run` pixels of the data, from its pixel `first`, to (x, y).
   Same addressing as gs_addr32/16/8/4. */
static void row_write(const GsPsmInfo *pi, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, const uint8_t *p,
                      uint32_t first, uint32_t run) {
    uint32_t k;

    y &= 2047;
    switch (pi->kind) {
    case GS_K32: {
        const uint16_t *row = gs_pg32[pi->layout == GS_L32Z][y & 31];
        uint32_t base = bp + (y >> 5) * bw * 32;
        p += first * 4;
        for (k = 0; k < run; k++) {
            uint32_t xx = (x + k) & 2047, v;
            memcpy(&v, p + k * 4, 4);
            GS_VRAM32[(((base + (xx >> 6) * 32) << 6) + row[xx & 63]) & 0xFFFFF] = v;
        }
        break;
    }
    case GS_K16: {
        const uint16_t *row = gs_pg16[pi->layout - GS_L16][y & 63];
        uint32_t base = bp + (y >> 6) * bw * 32;
        p += first * 2;
        for (k = 0; k < run; k++) {
            uint32_t xx = (x + k) & 2047;
            uint16_t v;
            memcpy(&v, p + k * 2, 2);
            GS_VRAM16[(((base + (xx >> 6) * 32) << 7) + row[xx & 63]) & 0x1FFFFF] = v;
        }
        break;
    }
    case GS_K8: {
        const uint16_t *row = gs_pg8[y & 63];
        uint32_t base = bp + (y >> 6) * gs_bw8(bw) * 32;
        p += first;
        for (k = 0; k < run; k++) {
            uint32_t xx = (x + k) & 2047;
            gs_vram[(((base + (xx >> 7) * 32) << 8) + row[xx & 127]) & 0x3FFFFF] = p[k];
        }
        break;
    }
    default: { /* GS_K4 */
        const uint16_t *row = gs_pg4[y & 127];
        uint32_t base = bp + (y >> 7) * gs_bw8(bw) * 32;
        for (k = 0; k < run; k++) {
            uint32_t xx = (x + k) & 2047, j = first + k;
            uint32_t a = (((base + (xx >> 7) * 32) << 9) + row[xx & 127]) & 0x7FFFFF;
            uint32_t v = (p[j >> 1] >> ((j & 1) * 4)) & 15;
            uint8_t *b = &gs_vram[a >> 1];
            if (a & 1)
                *b = (uint8_t)((*b & 0x0F) | (v << 4));
            else
                *b = (uint8_t)((*b & 0xF0) | v);
        }
        break;
    }
    }
}

/* Moves the transfer on by up to `avail` pixels, a row at a time,
   writing them unless `skip`; returns how many */
static uint32_t runs(GsTransfer *t, const GsPsmInfo *pi, const uint8_t *p, uint32_t avail, int skip) {
    uint32_t done = 0;
    while (done < avail && t->active) {
        uint32_t run = t->w - (t->x - t->sx);
        if (run > t->left)
            run = t->left;
        if (run > avail - done)
            run = avail - done;
        if (!skip)
            row_write(pi, t->bp, t->bw, t->x, t->y, p, done, run);
        done += run;
        t->x += run;
        t->left -= run;
        if (t->x - t->sx >= t->w) {
            t->x = t->sx;
            t->y++;
        }
        if (t->left == 0)
            t->active = 0;
    }
    return done;
}

void gs_upload_rows(const GsUpload *u, const GsBand *b) {
    const GsPsmInfo *pi = &gs_psm[u->psm & 63];
    uint32_t done = 0, x = u->x, y = u->y;
    while (done < u->npix) {
        uint32_t run = u->w - (x - u->sx);
        if (run > u->npix - done)
            run = u->npix - done;
        if (gs_band_owns(b, (int)(y & 0x7FFFFFFF)))
            row_write(pi, u->bp, u->bw, x, y, u->data, done, run);
        done += run;
        x = u->sx;
        y++;
    }
}

void gs_trx_data(const uint8_t *p, uint32_t n) {
    GsTransfer *t = &gs.up;
    const GsPsmInfo *pi;
    uint32_t bits, bpp, v;
    int fast;

    if (!t->active)
        return;
    if (gs_backend)
        gs_backend->wrote(&up_pages);
    gs_ws_upload_data(p, &up_pages);
    pi = &gs_psm[t->psm & 63];
    fast = pi->kind == GS_K32 || pi->kind == GS_K16 || pi->kind == GS_K8 || pi->kind == GS_K4;
    bits = trx_bits(t->psm);
    bpp = bits / 8;
    if (gs_mem_busy(&up_pages)) {
        /* Queued work still uses these pages: the workers write it, in
           order, unless it is a format or a piece they don't take */
        if (fast && t->npend == 0 && (bits == 4 || n % bpp == 0)) {
            GsUpload u;
            uint32_t npix = bits == 4 ? n * 2 : n / bpp;
            u.psm = t->psm;
            u.bp = t->bp;
            u.bw = t->bw;
            u.sx = t->sx;
            u.w = t->w;
            u.x = t->x;
            u.y = t->y;
            u.data = p;
            u.npix = runs(t, pi, p, npix, 1);
            gs_queue_upload(&u, bits == 4 ? (u.npix + 1) / 2 : u.npix * bpp, &up_pages);
            return;
        }
        gs_flush();
    }
    if (bits == 4) {
        if (fast) {
            /* two pixels a byte, low nibble first; a nibble left over when
               the transfer ends is dropped */
            runs(t, pi, p, n * 2, 0);
            return;
        }
        while (n-- && t->active) {
            uint8_t b = *p++;
            gs_write_pixel(t->psm, t->bp, t->bw, t->x, t->y, b & 15);
            trx_next(t);
            if (!t->active)
                break;
            gs_write_pixel(t->psm, t->bp, t->bw, t->x, t->y, b >> 4);
            trx_next(t);
        }
        return;
    }
    /* Finish a pixel split over two pieces of data */
    while (t->npend && n && t->active) {
        t->pend[t->npend++] = *p++;
        n--;
        if (t->npend == bpp) {
            v = 0;
            memcpy(&v, t->pend, bpp);
            gs_write_pixel(t->psm, t->bp, t->bw, t->x, t->y, v);
            trx_next(t);
            t->npend = 0;
        }
    }
    if (fast && t->active) {
        uint32_t done = runs(t, pi, p, n / bpp, 0);
        p += done * bpp;
        n -= done * bpp;
    }
    while (n >= bpp && t->active) {
        v = 0;
        memcpy(&v, p, bpp);
        p += bpp;
        n -= bpp;
        gs_write_pixel(t->psm, t->bp, t->bw, t->x, t->y, v);
        trx_next(t);
    }
    while (n && t->active)
        t->pend[t->npend++] = *p++, n--;
}

void grx_gs_read_image(void *dst, uint32_t qwc) {
    GsTransfer *t = &gs.down;
    uint8_t *o = dst;
    uint32_t n = qwc * 16, bits, bpp, v;

    gs_flush();
    if ((gs_backend || gs_ws_on) && t->active) {
        GsPages pg;
        memset(&pg, 0, sizeof(pg));
        gs_pages_rect(&pg, t->psm, t->bp, t->bw, t->sx, t->y, t->w, t->y + t->h > 2048 ? 2048 - t->y : t->h);
        if (gs_backend)
            gs_backend->reading(&pg);
        gs_ws_read_back(dst, n, &pg);
    }
    bits = trx_bits(t->psm);
    bpp = bits / 8;
    while (n && t->active) {
        if (bits == 4) {
            uint32_t lo = gs_read_pixel(t->psm, t->bp, t->bw, t->x, t->y), hi = 0;
            trx_next(t);
            if (t->active) {
                hi = gs_read_pixel(t->psm, t->bp, t->bw, t->x, t->y);
                trx_next(t);
            }
            *o++ = (uint8_t)(lo | (hi << 4));
            n--;
            continue;
        }
        /* A pixel may straddle two reads (24-bit) */
        while (t->npend && n) {
            *o++ = t->pend[8 - t->npend];
            t->npend--;
            n--;
        }
        if (!n)
            break;
        v = gs_read_pixel(t->psm, t->bp, t->bw, t->x, t->y);
        trx_next(t);
        if (n >= bpp) {
            memcpy(o, &v, bpp);
            o += bpp;
            n -= bpp;
        } else {
            memcpy(&t->pend[8 - bpp], &v, bpp);
            t->npend = bpp;
            while (t->npend && n) {
                *o++ = t->pend[8 - t->npend];
                t->npend--;
                n--;
            }
        }
    }
    while (n && t->npend) {
        *o++ = t->pend[8 - t->npend];
        t->npend--;
        n--;
    }
    if (n)
        memset(o, 0, n);
    if (gs_rec_active)
        gs_rec_read_image(dst, qwc);
}

/* ---- CLUT ---- */

static int psm_indexed(uint32_t psm) {
    return psm == GS_PSMT8 || psm == GS_PSMT8H || psm == GS_PSMT4 || psm == GS_PSMT4HL || psm == GS_PSMT4HH;
}

/* Where CLUT entry i of n is read from */
static void clut_xy(uint32_t i, uint32_t n, uint32_t csm, uint32_t cou, uint32_t cov, uint32_t *x, uint32_t *y) {
    if (csm) {
        /* CSM2: one line of the buffer */
        *x = cou + i;
        *y = cov;
    } else if (n == 256) {
        /* CSM1: 16x16, entries 8-15 and 16-23 of each 32 swapped */
        uint32_t l = (i & ~0x18u) | ((i & 0x08) << 1) | ((i & 0x10) >> 1);
        *x = l & 15;
        *y = l >> 4;
    } else {
        *x = i & 7;
        *y = i >> 3;
    }
}

/* The pixel an upload writes at (x, y) of its buffer, if it writes it (and
   doesn't wrap around the 2048 x 2048 coordinates) */
static int upload_pixel(const GsUpload *u, uint32_t x, uint32_t y, uint32_t bpp, uint32_t *v) {
    uint32_t k0 = u->x - u->sx, idx, rows;
    if (u->w == 0 || u->sx + u->w > 2048 || x < u->sx || x >= u->sx + u->w || y < u->y)
        return 0;
    rows = (k0 + u->npix + u->w - 1) / u->w;
    if (u->y + rows > 2048 || y - u->y >= rows)
        return 0;
    idx = (y - u->y) * u->w + (x - u->sx);
    if (idx < k0 || idx - k0 >= u->npix)
        return 0;
    *v = 0;
    memcpy(v, u->data + (idx - k0) * bpp, bpp);
    return 1;
}

void gs_clut_load(uint64_t tex0) {
    uint32_t psm = GS_FIELD(tex0, 20, 6);
    uint32_t cbp = GS_FIELD(tex0, 37, 14), cpsm = GS_FIELD(tex0, 51, 4);
    uint32_t csm = GS_FIELD(tex0, 55, 1), csa = GS_FIELD(tex0, 56, 5);
    uint32_t cld = GS_FIELD(tex0, 61, 3);
    uint32_t n, i, cbw, cou, cov;
    int c32;
    const GsUpload *src;

    switch (cld) {
    case 1: break;
    case 2: gs.cbp0 = cbp; break;
    case 3: gs.cbp1 = cbp; break;
    case 4:
        if (cbp == gs.cbp0)
            return;
        gs.cbp0 = cbp;
        break;
    case 5:
        if (cbp == gs.cbp1)
            return;
        gs.cbp1 = cbp;
        break;
    default: return;
    }
    if (!psm_indexed(psm))
        return;
    n = (psm == GS_PSMT8 || psm == GS_PSMT8H) ? 256 : 16;
    c32 = (cpsm & 2) == 0; /* PSMCT32 (or 24); else PSMCT16/16S */
    cbw = GS_FIELD(gs.texclut, 0, 6);
    cou = GS_FIELD(gs.texclut, 6, 6) * 16;
    cov = GS_FIELD(gs.texclut, 12, 10);
    {
        GsPages pg;
        memset(&pg, 0, sizeof(pg));
        if (csm)
            gs_pages_rect(&pg, cpsm, cbp, cbw, cou, cov, n, 1);
        else if (n == 256)
            gs_pages_rect(&pg, cpsm, cbp, 1, 0, 0, 16, 16);
        else
            gs_pages_rect(&pg, cpsm, cbp, 1, 0, 0, 8, 2);
        /* The CLUT just uploaded and still in the queue: from its data */
        src = gs_queue_writer(&pg);
        if (src != NULL) {
            uint32_t bpp = cpsm == GS_PSMCT32 ? 4 : 2;
            if ((cpsm != GS_PSMCT32 && cpsm != GS_PSMCT16 && cpsm != GS_PSMCT16S) || src->psm != cpsm ||
                src->bp != cbp || src->bw != (csm ? cbw : 1))
                src = NULL;
            for (i = 0; src != NULL && i < n; i++) {
                uint32_t x, y, v;
                clut_xy(i, n, csm, cou, cov, &x, &y);
                if (!upload_pixel(src, x, y, bpp, &v))
                    src = NULL;
            }
        }
        if (src == NULL) {
            if (gs_backend)
                gs_backend->reading(&pg);
            else
                gs_mem_reading(&pg);
        }
    }
    for (i = 0; i < n; i++) {
        uint32_t x, y, v;
        clut_xy(i, n, csm, cou, cov, &x, &y);
        if (src != NULL)
            upload_pixel(src, x, y, cpsm == GS_PSMCT32 ? 4 : 2, &v);
        else
            v = gs_read_pixel(cpsm, cbp, csm ? cbw : 1, x, y);
        if (c32) {
            uint32_t pos = ((csa & 15) * 16 + i) & 255;
            gs.clut[pos] = (uint16_t)v;
            gs.clut[pos + 256] = (uint16_t)(v >> 16);
        } else {
            gs.clut[(csa * 16 + i) & 511] = (uint16_t)v;
        }
    }
}
