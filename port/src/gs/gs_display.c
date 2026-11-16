/* The GS's display: the two read circuits (DISPFB1/2, DISPLAY1/2), merged
   per PMODE over BGCOLOR, as one picture of 0xAARRGGBB pixels.

   Interlace is handled simply: whatever the field, the whole frame
   buffer area the circuit reads is shown (the game draws full 448-line
   frames in field mode). */
#include <stdlib.h>
#include "gs_internal.h"
#include "gs_draw.h"
#include "grx_gs.h"

static uint32_t *disp_buf;
static size_t disp_cap;

typedef struct Circuit {
    int on;
    uint32_t bp, bw, psm, dbx, dby;
    int dx, dy, w, h; /* position on screen in output pixels, size */
} Circuit;

static void circuit(Circuit *c, int on, uint64_t dispfb, uint64_t display) {
    uint32_t magh = GS_FIELD(display, 23, 4) + 1, magv = GS_FIELD(display, 27, 2) + 1;

    c->on = on;
    c->bp = GS_FIELD(dispfb, 0, 9) * 32;
    c->bw = GS_FIELD(dispfb, 9, 6);
    c->psm = GS_FIELD(dispfb, 15, 5);
    c->dbx = GS_FIELD(dispfb, 32, 11);
    c->dby = GS_FIELD(dispfb, 43, 11);
    c->dx = (int)(GS_FIELD(display, 0, 12) / magh);
    c->dy = (int)(GS_FIELD(display, 12, 11) / magv);
    c->w = (int)((GS_FIELD(display, 32, 12) + 1) / magh);
    c->h = (int)((GS_FIELD(display, 44, 11) + 1) / magv);
}

/* One row of a circuit's pixels as 0xAABBGGRR (alpha as stored, 0x80 =
   1.0 for 16-bit): n pixels from x on the circuit's row y */
static void circuit_row(const Circuit *c, int x, int y, int n, uint32_t *out) {
    uint32_t py = (c->dby + (uint32_t)y) & 2047, px = c->dbx + (uint32_t)x;
    int i;
    switch (c->psm) {
    case GS_PSMCT16:
    case GS_PSMCT16S: {
        const uint16_t *row = gs_pg16[c->psm == GS_PSMCT16 ? 0 : 1][py & 63];
        uint32_t base = c->bp + (py >> 6) * c->bw * 32;
        for (i = 0; i < n; i++) {
            uint32_t xx = (px + (uint32_t)i) & 2047;
            uint32_t v = GS_VRAM16[(((base + (xx >> 6) * 32) << 7) + row[xx & 63]) & 0x1FFFFF];
            out[i] = ((v & 0x1F) << 3) | ((v & 0x3E0) << 6) | ((v & 0x7C00) << 9) | ((v & 0x8000) ? 0x80000000u : 0);
        }
        break;
    }
    default: {
        /* PSMCT32, PSMCT24 (alpha 0xFF) and the undefined ones as PSMCT32 */
        const uint16_t *row = gs_pg32[0][py & 31];
        uint32_t base = c->bp + (py >> 5) * c->bw * 32;
        uint32_t or = c->psm == GS_PSMCT24 ? 0xFF000000u : 0;
        for (i = 0; i < n;) {
            uint32_t xx = (px + (uint32_t)i) & 2047;
            uint32_t pb = ((base + (xx >> 6) * 32) << 6), k = xx & 63, m = 64 - k;
            if (m > (uint32_t)(n - i))
                m = (uint32_t)(n - i);
            for (; m; m--, k++, i++)
                out[i] = GS_VRAM32[(pb + row[k]) & 0xFFFFF] | or;
        }
        break;
    }
    }
}

static inline uint32_t to_argb(uint32_t c) {
    return 0xFF000000u | ((c & 0xFF) << 16) | (c & 0xFF00) | ((c >> 16) & 0xFF);
}

int grx_gs_display(const uint32_t **pixels, int *width, int *height, int *pitch) {
    static uint32_t row1[4096], row2[4096];
    uint64_t pmode = gs_priv_read(GS_PRIV_PMODE);
    uint64_t bg = gs_priv_read(GS_PRIV_BGCOLOR);
    int en1 = GS_FIELD(pmode, 0, 1), en2 = GS_FIELD(pmode, 1, 1);
    int mmod = GS_FIELD(pmode, 5, 1), slbg = GS_FIELD(pmode, 7, 1);
    uint32_t alp = GS_FIELD(pmode, 8, 8);
    uint32_t bgc = (uint32_t)bg & 0xFFFFFF;
    Circuit c1, c2;
    int x0, y0, x1, y1, w, h, x, y;

    if (gs_backend)
        return gs_backend->display(pixels, width, height, pitch);
    circuit(&c1, en1, gs_priv_read(GS_PRIV_DISPFB1), gs_priv_read(GS_PRIV_DISPLAY1));
    circuit(&c2, en2, gs_priv_read(GS_PRIV_DISPFB2), gs_priv_read(GS_PRIV_DISPLAY2));
    if (!en1 && !en2)
        return 0;
    {
        /* Only the queued work on the buffers shown has to be finished */
        GsPages pg;
        memset(&pg, 0, sizeof(pg));
        if (en1 && c1.w > 0 && c1.h > 0)
            gs_pages_rect(&pg, c1.psm, c1.bp, c1.bw, c1.dbx, c1.dby, (uint32_t)c1.w, (uint32_t)c1.h);
        if (en2 && c2.w > 0 && c2.h > 0)
            gs_pages_rect(&pg, c2.psm, c2.bp, c2.bw, c2.dbx, c2.dby, (uint32_t)c2.w, (uint32_t)c2.h);
        gs_display_reading(&pg);
    }
    /* The picture spans the enabled circuits' rectangles */
    if (en1 && en2) {
        x0 = c1.dx < c2.dx ? c1.dx : c2.dx;
        y0 = c1.dy < c2.dy ? c1.dy : c2.dy;
        x1 = c1.dx + c1.w > c2.dx + c2.w ? c1.dx + c1.w : c2.dx + c2.w;
        y1 = c1.dy + c1.h > c2.dy + c2.h ? c1.dy + c1.h : c2.dy + c2.h;
    } else {
        const Circuit *c = en1 ? &c1 : &c2;
        x0 = c->dx;
        y0 = c->dy;
        x1 = c->dx + c->w;
        y1 = c->dy + c->h;
    }
    w = x1 - x0;
    h = y1 - y0;
    if (w <= 0 || h <= 0 || w > 4096 || h > 4096)
        return 0;
    if ((size_t)w * h > disp_cap) {
        uint32_t *nb = realloc(disp_buf, (size_t)w * h * 4);
        if (!nb)
            return 0;
        disp_buf = nb;
        disp_cap = (size_t)w * h;
    }
    for (y = 0; y < h; y++) {
        uint32_t *row = disp_buf + (size_t)y * w;
        int sy = y + y0;
        int r1 = en1 && sy >= c1.dy && sy < c1.dy + c1.h;
        int r2 = en2 && sy >= c2.dy && sy < c2.dy + c2.h;
        /* The columns each circuit covers on this row, in output pixels */
        int a1 = r1 ? (c1.dx > x0 ? c1.dx : x0) - x0 : 0, b1 = r1 ? (c1.dx + c1.w < x1 ? c1.dx + c1.w : x1) - x0 : 0;
        int a2 = r2 ? (c2.dx > x0 ? c2.dx : x0) - x0 : 0, b2 = r2 ? (c2.dx + c2.w < x1 ? c2.dx + c2.w : x1) - x0 : 0;
        if (b1 > a1)
            circuit_row(&c1, a1 + x0 - c1.dx, sy - c1.dy, b1 - a1, row1 + a1);
        else
            b1 = a1 = 0;
        if (b2 > a2 && !slbg)
            circuit_row(&c2, a2 + x0 - c2.dx, sy - c2.dy, b2 - a2, row2 + a2);
        else
            b2 = a2 = 0;
        if (b1 <= a1) {
            /* the background circuit (or colour) alone */
            for (x = 0; x < a2; x++)
                row[x] = to_argb(bgc);
            for (; x < b2; x++)
                row[x] = to_argb(row2[x]);
            for (; x < w; x++)
                row[x] = to_argb(bgc);
            continue;
        }
        for (x = 0; x < w; x++) {
            uint32_t back = (x >= a2 && x < b2) ? row2[x] : bgc;
            uint32_t out;
            if (x >= a1 && x < b1) {
                uint32_t p = row1[x];
                uint32_t a = mmod ? alp : (p >> 24);
                int k;
                if (a > 255)
                    a = 255;
                out = 0;
                if (!mmod) {
                    /* Pixel alpha: 0x80 is 1.0 */
                    a = a >= 0x80 ? 255 : a * 2;
                }
                for (k = 0; k < 24; k += 8) {
                    uint32_t f = (p >> k) & 255, b = (back >> k) & 255;
                    out |= ((f * a + b * (255 - a) + 127) / 255) << k;
                }
            } else {
                out = back;
            }
            row[x] = to_argb(out);
        }
    }
    *pixels = disp_buf;
    *width = w;
    *height = h;
    *pitch = w;
    return 1;
}
