/* The draw state and the rasteriser's entry points, shared by gs_draw.c
   (the rasteriser) and gs_queue.c (the queue of draws and the worker
   threads). */
#ifndef GS_DRAW_H
#define GS_DRAW_H

#include "gs_internal.h"

/* A set of the 512 pages (8 KB) of local memory */
typedef struct GsPages {
    uint32_t w[16];
} GsPages;

typedef struct GsTex {
    uint32_t tbp, tbw, psm, kind, layout;
    uint32_t tw, th;
    int32_t wmask, hmask;
    int wms, wmt;
    int32_t minu, maxu, minv, maxv;
    uint32_t ta0, ta1, aem;
    int mag_linear, min_linear, lcm, l;
    float k;
    uint32_t pal[256];
} GsTex;

/* Attributes at a pixel, and their steps */
typedef struct Attr {
    double z;
    float r, g, b, a, f, s, t, q;
} Attr;

typedef struct GsDraw GsDraw;
typedef void SpanFn(const GsDraw *d, int y, int x0, int x1, Attr a, const Attr *g);

/* Everything a draw depends on, from the registers at the time of the
   kick (gs_setup) */
struct GsDraw {
    int iip, tme, fge, abe, fst;
    int tcc, tfx;
    /* frame */
    uint32_t fbp, fbw, fbmsk;
    int f16, flay, fvalid, f24;
    uint32_t fbmsk16;
    /* Z */
    uint32_t zbp;
    int z16, zlay, z24, zte, ztst, zwrite, zon;
    uint32_t zmax;
    /* tests */
    int ate, atst, aref, afail, date, datm;
    /* blending */
    int ba, bb, bc, bd;
    int32_t fix;
    int pabe, colclamp, fba, dthe;
    int dimx[4][4];
    uint32_t fogcol;
    int scx0, scx1, scy0, scy1, scanmsk;
    int needs_lod;
    SpanFn *span;
    GsPages rtex; /* the pages the texture is read from (TME) */
    GsTex t;
};

/* The rows a thread draws: chunks of 1 << sh rows, chunk c going to
   band c % n; band k of n; and only rows ylo..yhi-1 */
typedef struct GsBand {
    int k, n, sh;
    int ylo, yhi;
} GsBand;

static inline int gs_band_owns(const GsBand *b, int y) {
    return y >= b->ylo && y < b->yhi && (b->n == 1 || (int)(((uint32_t)y >> b->sh) % (uint32_t)b->n) == b->k);
}

/* gs_draw.c */
void gs_setup(GsDraw *d);
void gs_render(const GsDraw *d, const GsBand *band, const GsVertex *v, uint32_t type);
/* The texel coordinates (u0, u1, v0, v1, inclusive, before wrapping) a
   textured sprite reads; 0 if it draws nothing */
int gs_sprite_texels(const GsDraw *d, const GsVertex *v0, const GsVertex *v1, int32_t r[6]);
void gs_sprite_row_texels(const GsDraw *d, const GsVertex *v0, const GsVertex *v1, int y, int32_t r[2]);

/* A piece of a host-to-local image transfer: npix pixels of data (from
   its first pixel; 4-bit: the low nibble of the first byte), going to the
   transfer's buffer from (x, y), rows w wide from column sx */
typedef struct GsUpload {
    uint32_t psm, bp, bw, sx, w, x, y, npix;
    const uint8_t *data;
} GsUpload;

/* gs_mem.c: write the rows of an upload a band owns (by row number) */
void gs_upload_rows(const GsUpload *u, const GsBand *b);

/* gs_queue.c: the pages a rectangle of pixels in a PSM covers, added to
   the set */
void gs_pages_rect(GsPages *s, uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t w,
                   uint32_t h);
/* gs_mem_reading for the display */
void gs_display_reading(const GsPages *p);
/* Whether queued work still writes or reads these pages */
int gs_mem_busy(const GsPages *p);
/* Queue an upload (its data is copied) that writes these pages */
void gs_queue_upload(const GsUpload *u, uint32_t bytes, const GsPages *dest);
/* The queued upload that is the last thing to write all these pages, if
   it isn't done yet */
const GsUpload *gs_queue_writer(const GsPages *p);

#endif
