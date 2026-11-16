/* The GS's drawing: primitive setup, rasterisation and the pixel
   pipeline (texture, fog, alpha test, destination alpha test, depth test,
   alpha blending, dithering, colour clamp, frame and Z writes).

   Draws run on worker threads (gs_queue.c): each primitive is queued
   with a snapshot of the draw state (GsDraw) it was kicked with, and every
   worker renders every queued primitive, in order, but only on its own
   rows (gs_draw.h, GsBand). A pixel is always computed the same way
   whichever thread computes it, so the output is the same as drawing
   everything in order on one thread; the queue makes sure no two threads
   touch the same memory out of order. */
#include <math.h>
#include "gs_internal.h"
#include "gs_draw.h"

/* 16-bit colour to 0xAABBGGRR, alpha per TEXA */
static inline uint32_t exp16(uint32_t c, uint32_t ta0, uint32_t ta1, uint32_t aem) {
    uint32_t rgb = ((c & 0x1F) << 3) | ((c & 0x3E0) << 6) | ((c & 0x7C00) << 9);
    uint32_t a = (c & 0x8000) ? ta1 : ((!aem || (c & 0x7FFF)) ? ta0 : 0);
    return rgb | (a << 24);
}

static SpanFn *const span_fns[16];

/* The highest texel coordinate a wrap mode can give */
static uint32_t wrap_max(int mode, uint32_t size_log2, uint32_t mn, uint32_t mx) {
    switch (mode) {
    case 0: case 1: return (1u << size_log2) - 1;
    case 2: return mn > mx ? mn : mx;
    default: return mn | mx;
    }
}

void gs_setup(GsDraw *D) {
    uint64_t attr = (gs.prmodecont & 1) ? gs.prim : gs.prmode;
    const GsContext *c;
    uint32_t fpsm, zpsm, i, j;
    GsTex *t = &D->t;

    D->iip = GS_FIELD(attr, 3, 1);
    D->tme = GS_FIELD(attr, 4, 1);
    D->fge = GS_FIELD(attr, 5, 1);
    D->abe = GS_FIELD(attr, 6, 1);
    D->fst = GS_FIELD(attr, 8, 1);
    c = &gs.ctx[GS_FIELD(attr, 9, 1)];

    fpsm = GS_FIELD(c->frame, 24, 6);
    D->fbp = GS_FIELD(c->frame, 0, 9) * 32;
    D->fbw = GS_FIELD(c->frame, 16, 6);
    D->fbmsk = (uint32_t)(c->frame >> 32);
    D->fvalid = 1;
    D->f16 = 0;
    D->f24 = 0;
    switch (fpsm) {
    case GS_PSMCT32: D->flay = 0; break;
    case GS_PSMCT24: D->flay = 0; D->f24 = 1; break;
    case GS_PSMZ32: D->flay = 1; break;
    case GS_PSMZ24: D->flay = 1; D->f24 = 1; break;
    case GS_PSMCT16: D->f16 = 1; D->flay = 0; break;
    case GS_PSMCT16S: D->f16 = 1; D->flay = 1; break;
    case GS_PSMZ16: D->f16 = 1; D->flay = 2; break;
    case GS_PSMZ16S: D->f16 = 1; D->flay = 3; break;
    default: D->fvalid = 0; break;
    }
    if (D->f24)
        D->fbmsk |= 0xFF000000u;
    D->fbmsk16 = ((D->fbmsk >> 3) & 0x1F) | ((D->fbmsk >> 6) & 0x3E0) | ((D->fbmsk >> 9) & 0x7C00) |
                 ((D->fbmsk >> 16) & 0x8000);

    zpsm = 0x30 | GS_FIELD(c->zbuf, 24, 4);
    D->zbp = GS_FIELD(c->zbuf, 0, 9) * 32;
    D->z16 = 0;
    D->z24 = 0;
    D->zmax = 0xFFFFFFFFu;
    switch (zpsm) {
    case GS_PSMZ24: D->z24 = 1; D->zlay = 1; D->zmax = 0xFFFFFF; break;
    case GS_PSMZ16: D->z16 = 1; D->zlay = 2; D->zmax = 0xFFFF; break;
    case GS_PSMZ16S: D->z16 = 1; D->zlay = 3; D->zmax = 0xFFFF; break;
    default: D->zlay = 1; break;
    }

    D->ate = GS_FIELD(c->test, 0, 1);
    D->atst = GS_FIELD(c->test, 1, 3);
    D->aref = GS_FIELD(c->test, 4, 8);
    D->afail = GS_FIELD(c->test, 12, 2);
    D->date = GS_FIELD(c->test, 14, 1) && !D->f24;
    D->datm = GS_FIELD(c->test, 15, 1);
    /* ZTE = 0 is not allowed on the GS; it is taken as no depth test and
       no Z write */
    D->zte = GS_FIELD(c->test, 16, 1);
    D->ztst = D->zte ? (int)GS_FIELD(c->test, 17, 2) : 1;
    D->zwrite = D->zte && !GS_FIELD(c->zbuf, 32, 1);
    if (!D->ate)
        D->atst = 1;

    D->ba = GS_FIELD(c->alpha, 0, 2);
    D->bb = GS_FIELD(c->alpha, 2, 2);
    D->bc = GS_FIELD(c->alpha, 4, 2);
    D->bd = GS_FIELD(c->alpha, 6, 2);
    D->fix = GS_FIELD(c->alpha, 32, 8);
    D->pabe = (int)gs.pabe;
    D->colclamp = (int)gs.colclamp;
    D->fba = (int)c->fba;
    D->dthe = (int)gs.dthe && D->f16;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            int v = (int)GS_FIELD(gs.dimx, i * 16 + j * 4, 3);
            D->dimx[i][j] = v >= 4 ? v - 8 : v;
        }
    D->fogcol = (uint32_t)gs.fogcol & 0xFFFFFF;
    D->scx0 = GS_FIELD(gs_ws_scissor(c->scissor), 0, 11);
    D->scx1 = GS_FIELD(gs_ws_scissor(c->scissor), 16, 11);
    D->scy0 = GS_FIELD(c->scissor, 32, 11);
    D->scy1 = GS_FIELD(c->scissor, 48, 11);
    D->scanmsk = (int)gs.scanmsk;

    D->needs_lod = 0;
    if (D->tme) {
        uint64_t tex0 = c->tex0;
        uint32_t mmin;
        t->tbp = GS_FIELD(tex0, 0, 14);
        t->tbw = GS_FIELD(tex0, 14, 6);
        t->psm = GS_FIELD(tex0, 20, 6);
        t->kind = gs_psm[t->psm].kind;
        t->layout = gs_psm[t->psm].layout;
        t->tw = GS_FIELD(tex0, 26, 4);
        t->th = GS_FIELD(tex0, 30, 4);
        if (t->tw > 10)
            t->tw = 10;
        if (t->th > 10)
            t->th = 10;
        t->wmask = (1 << t->tw) - 1;
        t->hmask = (1 << t->th) - 1;
        D->tcc = GS_FIELD(tex0, 34, 1);
        D->tfx = GS_FIELD(tex0, 35, 2);
        t->wms = GS_FIELD(c->clamp, 0, 2);
        t->wmt = GS_FIELD(c->clamp, 2, 2);
        t->minu = GS_FIELD(c->clamp, 4, 10);
        t->maxu = GS_FIELD(c->clamp, 14, 10);
        t->minv = GS_FIELD(c->clamp, 24, 10);
        t->maxv = GS_FIELD(c->clamp, 34, 10);
        t->ta0 = GS_FIELD(gs.texa, 0, 8);
        t->aem = GS_FIELD(gs.texa, 15, 1);
        t->ta1 = GS_FIELD(gs.texa, 32, 8);
        t->lcm = GS_FIELD(c->tex1, 0, 1);
        t->mag_linear = GS_FIELD(c->tex1, 5, 1);
        mmin = GS_FIELD(c->tex1, 6, 3);
        t->min_linear = mmin == 1 || mmin == 4 || mmin == 5;
        t->l = GS_FIELD(c->tex1, 19, 2);
        {
            int32_t k = (int32_t)GS_FIELD(c->tex1, 32, 12);
            if (k & 0x800)
                k -= 0x1000;
            t->k = (float)k / 16.0f;
        }
        D->needs_lod = t->mag_linear != t->min_linear;
        if (t->kind == GS_K8 || t->kind == GS_K8H || t->kind == GS_K4 || t->kind == GS_K4HL || t->kind == GS_K4HH) {
            uint32_t cpsm = GS_FIELD(tex0, 51, 4), csa = GS_FIELD(tex0, 56, 5);
            uint32_t n = (t->kind == GS_K8 || t->kind == GS_K8H) ? 256 : 16;
            for (i = 0; i < n; i++) {
                if ((cpsm & 2) == 0) {
                    uint32_t p = ((csa & 15) * 16 + i) & 255;
                    t->pal[i] = gs.clut[p] | ((uint32_t)gs.clut[p + 256] << 16);
                } else {
                    t->pal[i] = exp16(gs.clut[(csa * 16 + i) & 511], t->ta0, t->ta1, t->aem);
                }
            }
        }
        /* The pages the texture can be read from */
        memset(&D->rtex, 0, sizeof(D->rtex));
        gs_pages_rect(&D->rtex, t->psm, t->tbp, t->tbw, 0, 0,
                      wrap_max(t->wms, t->tw, (uint32_t)t->minu, (uint32_t)t->maxu) + 1,
                      wrap_max(t->wmt, t->th, (uint32_t)t->minv, (uint32_t)t->maxv) + 1);
    }
    D->zon = D->ztst != 1 || D->zwrite;
    D->span = span_fns[D->tme | (D->zon << 1) | ((D->abe || D->date) << 2) | (D->f16 << 3)];
}

/* ---- texture ---- */

static inline uint32_t texel(const GsTex *t, uint32_t u, uint32_t v) {
    uint32_t c;
    switch (t->kind) {
    case GS_K32:
        return GS_VRAM32[gs_addr32(t->layout == GS_L32Z, t->tbp, t->tbw, u, v)];
    case GS_K24:
        c = GS_VRAM32[gs_addr32(t->layout == GS_L32Z, t->tbp, t->tbw, u, v)] & 0xFFFFFF;
        return c | ((t->aem && !c) ? 0 : t->ta0 << 24);
    case GS_K16:
        return exp16(GS_VRAM16[gs_addr16(t->layout - GS_L16, t->tbp, t->tbw, u, v)], t->ta0, t->ta1, t->aem);
    case GS_K8:
        return t->pal[gs_vram[gs_addr8(t->tbp, t->tbw, u, v)]];
    case GS_K4: {
        uint32_t a = gs_addr4(t->tbp, t->tbw, u, v);
        return t->pal[(gs_vram[a >> 1] >> ((a & 1) * 4)) & 15];
    }
    case GS_K8H:
        return t->pal[GS_VRAM32[gs_addr32(0, t->tbp, t->tbw, u, v)] >> 24];
    case GS_K4HL:
        return t->pal[(GS_VRAM32[gs_addr32(0, t->tbp, t->tbw, u, v)] >> 24) & 15];
    default:
        return t->pal[GS_VRAM32[gs_addr32(0, t->tbp, t->tbw, u, v)] >> 28];
    }
}

static inline int32_t wrap(int32_t c, int mode, int32_t mask, int32_t mn, int32_t mx) {
    switch (mode) {
    case 0: return c & mask;
    case 1: return c < 0 ? 0 : c > mask ? mask : c;
    case 2: return c < mn ? mn : c > mx ? mx : c;
    default: return (c & mn) | mx;
    }
}

/* a + (b - a) * f / 256 on all four channels, two at a time */
static inline uint32_t lerp2(uint32_t a, uint32_t b, uint32_t f) {
    uint32_t g = 256 - f;
    uint32_t rb = ((((a & 0xFF00FF) * g + (b & 0xFF00FF) * f) >> 8) & 0xFF00FF);
    uint32_t ga = ((((a >> 8) & 0xFF00FF) * g + ((b >> 8) & 0xFF00FF) * f) & 0xFF00FF00u);
    return rb | ga;
}

/* u, v: texel coordinates with 8 fraction bits */
static inline uint32_t sample(const GsTex *t, int32_t u, int32_t v, int linear) {
    if (!linear) {
        int32_t x = wrap(u >> 8, t->wms, t->wmask, t->minu, t->maxu);
        int32_t y = wrap(v >> 8, t->wmt, t->hmask, t->minv, t->maxv);
        return texel(t, (uint32_t)x, (uint32_t)y);
    } else {
        int32_t uu = u - 128, vv = v - 128;
        int32_t fu = uu & 255, fv = vv & 255;
        /* A texel with weight 0 doesn't change the result: it isn't read
           (gs_sprite_texels counts on that) */
        int32_t x0 = wrap(uu >> 8, t->wms, t->wmask, t->minu, t->maxu);
        int32_t x1 = wrap((uu >> 8) + (fu != 0), t->wms, t->wmask, t->minu, t->maxu);
        int32_t y0 = wrap(vv >> 8, t->wmt, t->hmask, t->minv, t->maxv);
        int32_t y1 = wrap((vv >> 8) + (fv != 0), t->wmt, t->hmask, t->minv, t->maxv);
        uint32_t c00 = texel(t, x0, y0), c01 = texel(t, x1, y0);
        uint32_t c10 = texel(t, x0, y1), c11 = texel(t, x1, y1);
        return lerp2(lerp2(c00, c01, (uint32_t)fu), lerp2(c10, c11, (uint32_t)fu), (uint32_t)fv);
    }
}

/* One row of a texture: the texel address of column x is that of
   gs_addr32/16/8/4 with the row's part worked out once */
typedef struct TexRow {
    const uint16_t *tab;
    uint32_t base;
} TexRow;

static inline __attribute__((always_inline)) void trow_init(const GsTex *t, uint32_t y, TexRow *r) {
    y &= 2047;
    switch (t->layout) {
    case GS_L32:
    case GS_L32Z:
        r->tab = gs_pg32[t->layout == GS_L32Z][y & 31];
        r->base = t->tbp + (y >> 5) * t->tbw * 32;
        break;
    case GS_L8:
        r->tab = gs_pg8[y & 63];
        r->base = t->tbp + (y >> 6) * gs_bw8(t->tbw) * 32;
        break;
    case GS_L4:
        r->tab = gs_pg4[y & 127];
        r->base = t->tbp + (y >> 7) * gs_bw8(t->tbw) * 32;
        break;
    default:
        r->tab = gs_pg16[t->layout - GS_L16][y & 63];
        r->base = t->tbp + (y >> 6) * t->tbw * 32;
        break;
    }
}

/* As texel(), along a row */
static inline __attribute__((always_inline)) uint32_t trow_texel(const GsTex *t, const TexRow *r, uint32_t x) {
    uint32_t c, a;
    x &= 2047;
    switch (t->kind) {
    case GS_K32:
        return GS_VRAM32[(((r->base + (x >> 6) * 32) << 6) + r->tab[x & 63]) & 0xFFFFF];
    case GS_K24:
        c = GS_VRAM32[(((r->base + (x >> 6) * 32) << 6) + r->tab[x & 63]) & 0xFFFFF] & 0xFFFFFF;
        return c | ((t->aem && !c) ? 0 : t->ta0 << 24);
    case GS_K16:
        return exp16(GS_VRAM16[(((r->base + (x >> 6) * 32) << 7) + r->tab[x & 63]) & 0x1FFFFF], t->ta0, t->ta1,
                     t->aem);
    case GS_K8:
        return t->pal[gs_vram[(((r->base + (x >> 7) * 32) << 8) + r->tab[x & 127]) & 0x3FFFFF]];
    case GS_K4:
        a = (((r->base + (x >> 7) * 32) << 9) + r->tab[x & 127]) & 0x7FFFFF;
        return t->pal[(gs_vram[a >> 1] >> ((a & 1) * 4)) & 15];
    case GS_K8H:
        return t->pal[GS_VRAM32[(((r->base + (x >> 6) * 32) << 6) + r->tab[x & 63]) & 0xFFFFF] >> 24];
    case GS_K4HL:
        return t->pal[(GS_VRAM32[(((r->base + (x >> 6) * 32) << 6) + r->tab[x & 63]) & 0xFFFFF] >> 24) & 15];
    default:
        return t->pal[GS_VRAM32[(((r->base + (x >> 6) * 32) << 6) + r->tab[x & 63]) & 0xFFFFF] >> 28];
    }
}

/* ---- the pixel pipeline ---- */

static inline void attr_step(Attr *a, const Attr *d, float k) {
    a->z += d->z * k;
    a->r += d->r * k;
    a->g += d->g * k;
    a->b += d->b * k;
    a->a += d->a * k;
    a->f += d->f * k;
    a->s += d->s * k;
    a->t += d->t * k;
    a->q += d->q * k;
}

static inline int clamp255(int v) {
    return v < 0 ? 0 : v > 255 ? 255 : v;
}

static inline int ftoi8(float v) {
    int i = (int)v;
    return i < 0 ? 0 : i > 255 ? 255 : i;
}

#define FORCEINLINE static inline __attribute__((always_inline))

FORCEINLINE int32_t ffloor(float f) {
    int32_t i = (int32_t)f;
    return i - (f < (float)i);
}
FORCEINLINE int32_t dfloor(double f) {
    int32_t i = (int32_t)f;
    return i - (f < (double)i);
}

/* One span of pixels, x0..x1-1 on row y (already clipped), with
   compile-time switches for the costly stages: TME texture, ZON depth
   test or write, RD destination read (blending, DATE), F16 16-bit frame */
FORCEINLINE void span_impl(const GsDraw *d, int y, int x0, int x1, Attr a, const Attr *g, const int TME,
                           const int ZON, const int RD, const int F16) {
    const GsTex *t = &d->t;
    int x;
    float tw = (float)(1 << t->tw), th = (float)(1 << t->th);
    int flatc = !d->iip;
    int cr = 0, cg = 0, cb = 0, ca = 0;
    const int *dimrow = d->dimx[y & 3];
    const uint16_t *frow, *zrow = 0;
    uint32_t fbase, zbase = 0;
    int64_t zfix = 0, dz = 0;
    int32_t ufix = 0, vfix = 0, du = 0, dv = 0;
    int linear = t->mag_linear;
    /* The texture row(s) when V and the filter are the same all along the
       span (sprites): only U changes per pixel */
    int trows = 0;
    int32_t tfv = 0;
    float iqc = 0.0f;
    TexRow tr0, tr1;

    if (F16) {
        frow = gs_pg16[d->flay][y & 63];
        fbase = d->fbp + (uint32_t)(y >> 6) * d->fbw * 32;
    } else {
        frow = gs_pg32[d->flay][y & 31];
        fbase = d->fbp + (uint32_t)(y >> 5) * d->fbw * 32;
    }
    if (ZON) {
        if (d->z16) {
            zrow = gs_pg16[d->zlay][y & 63];
            zbase = d->zbp + (uint32_t)(y >> 6) * d->fbw * 32;
        } else {
            zrow = gs_pg32[1][y & 31];
            zbase = d->zbp + (uint32_t)(y >> 5) * d->fbw * 32;
        }
        zfix = (int64_t)(a.z * 65536.0);
        dz = (int64_t)(g->z * 65536.0);
    }
    if (TME && d->fst) {
        /* affine: texel coordinates with 16 fraction bits */
        ufix = dfloor((double)a.s * 65536.0);
        vfix = dfloor((double)a.t * 65536.0);
        du = dfloor((double)g->s * 65536.0 + 0.5);
        dv = dfloor((double)g->t * 65536.0 + 0.5);
    }
    if (flatc) {
        cr = ftoi8(a.r);
        cg = ftoi8(a.g);
        cb = ftoi8(a.b);
        ca = ftoi8(a.a);
    }
    if (TME) {
        /* Q the same all along (and not 0, whose sign an add can change) */
        int qconst = g->q == 0.0f && a.q == a.q && a.q != 0.0f;
        int vconst = d->fst ? dv == 0 : (qconst && g->t == 0.0f);
        int lconst = !d->needs_lod || t->lcm || qconst;
        if (vconst && lconst) {
            int32_t v;
            if (d->needs_lod) {
                float lod;
                if (t->lcm)
                    lod = t->k;
                else
                    lod = log2f(1.0f / fabsf(a.q)) * (float)(1 << t->l) + t->k;
                linear = lod > 0.0f ? t->min_linear : t->mag_linear;
            }
            if (d->fst) {
                v = vfix >> 8;
            } else {
                float fv;
                iqc = 1.0f / a.q;
                fv = a.t * iqc * th;
                fv = fv < 1.0e6f ? (fv > -1.0e6f ? fv : -1.0e6f) : 1.0e6f;
                v = ffloor(fv * 256.0f);
            }
            if (!linear) {
                trow_init(t, (uint32_t)wrap(v >> 8, t->wmt, t->hmask, t->minv, t->maxv), &tr0);
            } else {
                int32_t vv = v - 128;
                tfv = vv & 255;
                trow_init(t, (uint32_t)wrap(vv >> 8, t->wmt, t->hmask, t->minv, t->maxv), &tr0);
                trow_init(t, (uint32_t)wrap((vv >> 8) + (tfv != 0), t->wmt, t->hmask, t->minv, t->maxv), &tr1);
            }
            trows = 1;
        }
    }

    for (x = x0; x < x1; x++, a.r += g->r, a.g += g->g, a.b += g->b, a.a += g->a, a.f += g->f, a.s += g->s,
        a.t += g->t, a.q += g->q, zfix += dz, ufix += du, vfix += dv) {
        uint32_t fa, za = 0, dst = 0, zs = 0;
        int r, gg, b, al, wfb = 1, wz = ZON && d->zwrite, rgbonly = 0;

        if (F16)
            fa = (((fbase + (uint32_t)(x >> 6) * 32) << 7) + frow[x & 63]) & 0x1FFFFF;
        else
            fa = (((fbase + (uint32_t)(x >> 6) * 32) << 6) + frow[x & 63]) & 0xFFFFF;
        if (RD) {
            if (F16) {
                uint32_t c = GS_VRAM16[fa];
                dst = ((c & 0x1F) << 3) | ((c & 0x3E0) << 6) | ((c & 0x7C00) << 9) | ((c & 0x8000) << 16);
            } else {
                dst = GS_VRAM32[fa];
                if (d->f24)
                    dst = (dst & 0xFFFFFF) | 0x80000000u;
            }
            if (d->date && ((dst >> 31) != (uint32_t)d->datm))
                continue;
        }

        if (ZON) {
            int64_t zi = zfix >> 16;
            zs = zi <= 0 ? 0 : zi >= (int64_t)d->zmax ? d->zmax : (uint32_t)zi;
            if (d->z16)
                za = (((zbase + (uint32_t)(x >> 6) * 32) << 7) + zrow[x & 63]) & 0x1FFFFF;
            else
                za = (((zbase + (uint32_t)(x >> 6) * 32) << 6) + zrow[x & 63]) & 0xFFFFF;
            if (d->ztst != 1) {
                uint32_t zd;
                if (d->ztst == 0)
                    continue;
                zd = d->z16 ? GS_VRAM16[za] : d->z24 ? (GS_VRAM32[za] & 0xFFFFFF) : GS_VRAM32[za];
                if (d->ztst == 2 ? zs < zd : zs <= zd)
                    continue;
            }
        }

        /* colour */
        if (flatc) {
            r = cr; gg = cg; b = cb; al = ca;
        } else {
            r = ftoi8(a.r); gg = ftoi8(a.g); b = ftoi8(a.b); al = ftoi8(a.a);
        }
        if (TME) {
            int32_t u, v;
            uint32_t tc;
            int tr, tg, tb, ta;
            if (trows) {
                if (d->fst) {
                    u = ufix >> 8;
                } else {
                    float fu = a.s * iqc * tw;
                    fu = fu < 1.0e6f ? (fu > -1.0e6f ? fu : -1.0e6f) : 1.0e6f;
                    u = ffloor(fu * 256.0f);
                }
                if (!linear) {
                    tc = trow_texel(t, &tr0, (uint32_t)wrap(u >> 8, t->wms, t->wmask, t->minu, t->maxu));
                } else {
                    /* as sample(): a texel with weight 0 isn't read */
                    int32_t uu = u - 128, fu = uu & 255;
                    uint32_t xa = (uint32_t)wrap(uu >> 8, t->wms, t->wmask, t->minu, t->maxu), xb, l0;
                    if (fu) {
                        xb = (uint32_t)wrap((uu >> 8) + 1, t->wms, t->wmask, t->minu, t->maxu);
                        l0 = lerp2(trow_texel(t, &tr0, xa), trow_texel(t, &tr0, xb), (uint32_t)fu);
                        if (tfv)
                            tc = lerp2(l0, lerp2(trow_texel(t, &tr1, xa), trow_texel(t, &tr1, xb), (uint32_t)fu),
                                       (uint32_t)tfv);
                        else
                            tc = l0;
                    } else {
                        l0 = trow_texel(t, &tr0, xa);
                        tc = tfv ? lerp2(l0, trow_texel(t, &tr1, xa), (uint32_t)tfv) : l0;
                    }
                }
                goto have_tc;
            }
            if (d->fst) {
                u = ufix >> 8;
                v = vfix >> 8;
            } else {
                float iq = 1.0f / a.q;
                float fu = a.s * iq * tw, fv = a.t * iq * th;
                /* (and Q = 0's NaN to the limit) */
                fu = fu < 1.0e6f ? (fu > -1.0e6f ? fu : -1.0e6f) : 1.0e6f;
                fv = fv < 1.0e6f ? (fv > -1.0e6f ? fv : -1.0e6f) : 1.0e6f;
                u = ffloor(fu * 256.0f);
                v = ffloor(fv * 256.0f);
            }
            if (d->needs_lod) {
                float lod;
                if (t->lcm)
                    lod = t->k;
                else
                    lod = log2f(1.0f / fabsf(a.q)) * (float)(1 << t->l) + t->k;
                linear = lod > 0.0f ? t->min_linear : t->mag_linear;
            }
            tc = sample(t, u, v, linear);
        have_tc:
            tr = tc & 255;
            tg = (tc >> 8) & 255;
            tb = (tc >> 16) & 255;
            ta = tc >> 24;
            switch (d->tfx) {
            case 0: /* MODULATE */
                r = clamp255((tr * r) >> 7);
                gg = clamp255((tg * gg) >> 7);
                b = clamp255((tb * b) >> 7);
                if (d->tcc)
                    al = clamp255((ta * al) >> 7);
                break;
            case 1: /* DECAL */
                r = tr; gg = tg; b = tb;
                if (d->tcc)
                    al = ta;
                break;
            case 2: /* HIGHLIGHT */
                r = clamp255(((tr * r) >> 7) + al);
                gg = clamp255(((tg * gg) >> 7) + al);
                b = clamp255(((tb * b) >> 7) + al);
                if (d->tcc)
                    al = clamp255(ta + al);
                break;
            default: /* HIGHLIGHT2 */
                r = clamp255(((tr * r) >> 7) + al);
                gg = clamp255(((tg * gg) >> 7) + al);
                b = clamp255(((tb * b) >> 7) + al);
                if (d->tcc)
                    al = ta;
                break;
            }
        }
        if (d->fge) {
            int f = ftoi8(a.f);
            r = (f * r + (255 - f) * (int)(d->fogcol & 255)) >> 8;
            gg = (f * gg + (255 - f) * (int)((d->fogcol >> 8) & 255)) >> 8;
            b = (f * b + (255 - f) * (int)((d->fogcol >> 16) & 255)) >> 8;
        }

        /* alpha test */
        if (d->atst != 1) {
            int pass;
            switch (d->atst) {
            case 0: pass = 0; break;
            case 2: pass = al < d->aref; break;
            case 3: pass = al <= d->aref; break;
            case 4: pass = al == d->aref; break;
            case 5: pass = al >= d->aref; break;
            case 6: pass = al > d->aref; break;
            default: pass = al != d->aref; break;
            }
            if (!pass) {
                switch (d->afail) {
                case 0: continue;
                case 1: wz = 0; break;
                case 2: wfb = 0; break;
                default: wz = 0; rgbonly = 1; break;
                }
            }
        }

        if (ZON && wz) {
            if (d->z16)
                GS_VRAM16[za] = (uint16_t)zs;
            else if (d->z24)
                GS_VRAM32[za] = (GS_VRAM32[za] & 0xFF000000u) | zs;
            else
                GS_VRAM32[za] = zs;
        }
        if (!wfb)
            continue;

        /* blend: ((A - B) * C >> 7) + D */
        if (RD && d->abe && (!d->pabe || (al & 0x80))) {
            int cdr = (int)(dst & 255), cdg = (int)((dst >> 8) & 255), cdb = (int)((dst >> 16) & 255);
            int cc = d->bc == 0 ? al : d->bc == 1 ? (int)(dst >> 24) : d->fix;
            int ar = d->ba == 0 ? r : d->ba == 1 ? cdr : 0, ag = d->ba == 0 ? gg : d->ba == 1 ? cdg : 0;
            int ab = d->ba == 0 ? b : d->ba == 1 ? cdb : 0;
            int br = d->bb == 0 ? r : d->bb == 1 ? cdr : 0, bg = d->bb == 0 ? gg : d->bb == 1 ? cdg : 0;
            int bb = d->bb == 0 ? b : d->bb == 1 ? cdb : 0;
            int dr = d->bd == 0 ? r : d->bd == 1 ? cdr : 0, dg = d->bd == 0 ? gg : d->bd == 1 ? cdg : 0;
            int db = d->bd == 0 ? b : d->bd == 1 ? cdb : 0;
            r = (((ar - br) * cc) >> 7) + dr;
            gg = (((ag - bg) * cc) >> 7) + dg;
            b = (((ab - bb) * cc) >> 7) + db;
        }
        if (F16 && d->dthe) {
            int dm = dimrow[x & 3];
            r += dm;
            gg += dm;
            b += dm;
        }
        /* Only blending and dithering can leave 0..255 */
        if ((RD && d->abe) || (F16 && d->dthe)) {
            if (d->colclamp) {
                r = clamp255(r);
                gg = clamp255(gg);
                b = clamp255(b);
            } else {
                r &= 255;
                gg &= 255;
                b &= 255;
            }
        }
        if (d->fba)
            al |= 0x80;

        if (F16) {
            uint32_t m = d->fbmsk16 | (rgbonly ? 0x8000 : 0);
            uint32_t nv = (uint32_t)((r >> 3) | ((gg >> 3) << 5) | ((b >> 3) << 10) | ((al & 0x80) << 8));
            GS_VRAM16[fa] = (uint16_t)((GS_VRAM16[fa] & m) | (nv & ~m));
        } else {
            uint32_t m = d->fbmsk | (rgbonly ? 0xFF000000u : 0);
            uint32_t nv = (uint32_t)r | ((uint32_t)gg << 8) | ((uint32_t)b << 16) | ((uint32_t)al << 24);
            if (m)
                GS_VRAM32[fa] = (GS_VRAM32[fa] & m) | (nv & ~m);
            else
                GS_VRAM32[fa] = nv;
        }
    }
}

#define SPAN_VARIANT(n, tme, zon, rd, f16)                                                   \
    static void span_##n(const GsDraw *d, int y, int x0, int x1, Attr a, const Attr *g) { \
        span_impl(d, y, x0, x1, a, g, tme, zon, rd, f16);                                  \
    }
SPAN_VARIANT(0, 0, 0, 0, 0)
SPAN_VARIANT(1, 1, 0, 0, 0)
SPAN_VARIANT(2, 0, 1, 0, 0)
SPAN_VARIANT(3, 1, 1, 0, 0)
SPAN_VARIANT(4, 0, 0, 1, 0)
SPAN_VARIANT(5, 1, 0, 1, 0)
SPAN_VARIANT(6, 0, 1, 1, 0)
SPAN_VARIANT(7, 1, 1, 1, 0)
SPAN_VARIANT(8, 0, 0, 0, 1)
SPAN_VARIANT(9, 1, 0, 0, 1)
SPAN_VARIANT(10, 0, 1, 0, 1)
SPAN_VARIANT(11, 1, 1, 0, 1)
SPAN_VARIANT(12, 0, 0, 1, 1)
SPAN_VARIANT(13, 1, 0, 1, 1)
SPAN_VARIANT(14, 0, 1, 1, 1)
SPAN_VARIANT(15, 1, 1, 1, 1)
static SpanFn *const span_fns[16] = {span_0, span_1, span_2,  span_3,  span_4,  span_5,  span_6,  span_7,
                                     span_8, span_9, span_10, span_11, span_12, span_13, span_14, span_15};

static void span(const GsDraw *d, int y, int x0, int x1, Attr a, const Attr *g) {
    if (y < d->scy0 || y > d->scy1)
        return;
    if ((d->scanmsk == 2 && (y & 1)) || (d->scanmsk == 3 && !(y & 1)))
        return;
    if (x0 < d->scx0) {
        attr_step(&a, g, (float)(d->scx0 - x0));
        x0 = d->scx0;
    }
    if (x1 > d->scx1 + 1)
        x1 = d->scx1 + 1;
    if (x0 >= x1)
        return;
    d->span(d, y, x0, x1, a, g);
}

/* ---- primitives ---- */

/* The first row at or after y that the band owns; *run_end: where that
   run of its rows ends */
static inline int band_next(const GsBand *b, int y, int *run_end) {
    uint32_t c, r;
    if (b->n == 1) {
        *run_end = 0x7FFFFFFF;
        return y;
    }
    c = (uint32_t)y >> b->sh;
    r = c % (uint32_t)b->n;
    if ((int)r != b->k) {
        c += ((uint32_t)b->k + (uint32_t)b->n - r) % (uint32_t)b->n;
        y = (int)(c << b->sh);
    }
    *run_end = (int)((c + 1) << b->sh);
    return y;
}

static void vert_attr(const GsDraw *d, Attr *a, const GsVertex *v) {
    a->z = (double)v->z;
    a->r = (float)(v->rgba & 255);
    a->g = (float)((v->rgba >> 8) & 255);
    a->b = (float)((v->rgba >> 16) & 255);
    a->a = (float)(v->rgba >> 24);
    a->f = (float)v->f;
    if (d->fst) {
        a->s = (float)v->u / 16.0f;
        a->t = (float)v->v / 16.0f;
        a->q = v->q;
    } else {
        a->s = v->s;
        a->t = v->t;
        a->q = v->q;
    }
}

static void set_flat_colour(Attr *a, Attr *g, const GsVertex *flat) {
    a->r = (float)(flat->rgba & 255);
    a->g = (float)((flat->rgba >> 8) & 255);
    a->b = (float)((flat->rgba >> 16) & 255);
    a->a = (float)(flat->rgba >> 24);
    g->r = g->g = g->b = g->a = 0;
}

static inline int64_t ceil_div(int64_t n, int64_t d) {
    return n >= 0 ? (n + d - 1) / d : -((-n) / d);
}

/* First pixel at or right of an edge at scanline Y (1/16 units) */
static inline int edge_x(const GsVertex *a, const GsVertex *b, int64_t Y) {
    /* Both terms are integers below 2^35, so the double quotient is
       exact where it is an integer and its ceiling is right */
    double dy = (double)(b->y - a->y);
    double num = (double)a->x * dy + (double)(Y - a->y) * (double)(b->x - a->x);
    double q = num / (dy * 16.0);
    int i = (int)q;
    return i + (q > (double)i);
}

static void draw_triangle(const GsDraw *d, const GsBand *band, const GsVertex *p0, const GsVertex *p1,
                          const GsVertex *p2) {
    const GsVertex *v[3] = {p0, p1, p2}, *tmp;
    Attr A[3], gx, gy, base;
    int64_t area;
    double inv;
    int ys, ye, y, i, run_end;
    float *fa0, *fa1, *fa2, *fgx, *fgy;
    double ex1, ey1, ex2, ey2;

    /* sort by y */
    if (v[1]->y < v[0]->y) tmp = v[0], v[0] = v[1], v[1] = tmp;
    if (v[2]->y < v[1]->y) tmp = v[1], v[1] = v[2], v[2] = tmp;
    if (v[1]->y < v[0]->y) tmp = v[0], v[0] = v[1], v[1] = tmp;
    area = (int64_t)(v[1]->x - v[0]->x) * (v[2]->y - v[0]->y) - (int64_t)(v[2]->x - v[0]->x) * (v[1]->y - v[0]->y);
    if (area == 0)
        return;
    ys = (int)ceil_div(v[0]->y, 16);
    ye = (int)ceil_div(v[2]->y, 16); /* exclusive */
    if (ys < d->scy0)
        ys = d->scy0;
    if (ye > d->scy1 + 1)
        ye = d->scy1 + 1;
    if (ys < band->ylo)
        ys = band->ylo;
    if (ye > band->yhi)
        ye = band->yhi;
    ys = band_next(band, ys, &run_end);
    if (ys >= ye)
        return;
    for (i = 0; i < 3; i++)
        vert_attr(d, &A[i], v[i]);

    /* gradients per pixel */
    ex1 = (double)(v[1]->x - v[0]->x) / 16.0;
    ey1 = (double)(v[1]->y - v[0]->y) / 16.0;
    ex2 = (double)(v[2]->x - v[0]->x) / 16.0;
    ey2 = (double)(v[2]->y - v[0]->y) / 16.0;
    inv = 1.0 / (ex1 * ey2 - ex2 * ey1);
    {
        double d1 = A[1].z - A[0].z, d2 = A[2].z - A[0].z;
        gx.z = (d1 * ey2 - d2 * ey1) * inv;
        gy.z = (ex1 * d2 - ex2 * d1) * inv;
    }
    fa0 = &A[0].r;
    fa1 = &A[1].r;
    fa2 = &A[2].r;
    fgx = &gx.r;
    fgy = &gy.r;
    for (i = 0; i < 8; i++) {
        double d1 = (double)fa1[i] - fa0[i], d2 = (double)fa2[i] - fa0[i];
        fgx[i] = (float)((d1 * ey2 - d2 * ey1) * inv);
        fgy[i] = (float)((ex1 * d2 - ex2 * d1) * inv);
    }
    base = A[0];
    if (!d->iip) {
        set_flat_colour(&base, &gx, p2);
        gy.r = gy.g = gy.b = gy.a = 0;
    }

    for (y = ys; y < ye; y++) {
        int64_t Y;
        int xl, xr, xs;
        const GsVertex *sa, *sb;
        Attr a;
        double ox, oy;
        float *pa, *pb, *px, *py;

        if (y >= run_end) {
            y = band_next(band, y, &run_end);
            if (y >= ye)
                break;
        }
        Y = (int64_t)y * 16;
        if (Y < v[1]->y)
            sa = v[0], sb = v[1];
        else
            sa = v[1], sb = v[2];
        xs = edge_x(sa, sb, Y);
        xl = edge_x(v[0], v[2], Y);
        if (area > 0) {
            xr = xs;
        } else {
            xr = xl;
            xl = xs;
        }
        if (xl < d->scx0)
            xl = d->scx0;
        if (xr > d->scx1 + 1)
            xr = d->scx1 + 1;
        if (xl >= xr)
            continue;
        ox = xl - v[0]->x / 16.0;
        oy = y - v[0]->y / 16.0;
        a.z = base.z + gx.z * ox + gy.z * oy;
        pa = &a.r;
        pb = &base.r;
        px = &gx.r;
        py = &gy.r;
        for (i = 0; i < 8; i++)
            pa[i] = (float)(pb[i] + px[i] * ox + py[i] * oy);
        span(d, y, xl, xr, a, &gx);
    }
}

static void draw_sprite(const GsDraw *d, const GsBand *band, const GsVertex *v0, const GsVertex *v1) {
    Attr a0, a1, g, base;
    int32_t x0 = v0->x, x1 = v1->x, y0 = v0->y, y1 = v1->y;
    int px0, px1, py0, py1, y, run_end;
    float dsdx, dtdy;

    vert_attr(d, &a0, v0);
    vert_attr(d, &a1, v1);
    if (x0 > x1) {
        int32_t tx = x0;
        float ts = a0.s;
        x0 = x1;
        x1 = tx;
        a0.s = a1.s;
        a1.s = ts;
    }
    if (y0 > y1) {
        int32_t ty = y0;
        float tt = a0.t;
        y0 = y1;
        y1 = ty;
        a0.t = a1.t;
        a1.t = tt;
    }
    if (x0 == x1 || y0 == y1)
        return;
    px0 = (int)ceil_div(x0, 16);
    px1 = (int)ceil_div(x1, 16);
    py0 = (int)ceil_div(y0, 16);
    py1 = (int)ceil_div(y1, 16);
    dsdx = (a1.s - a0.s) * 16.0f / (float)(x1 - x0);
    dtdy = (a1.t - a0.t) * 16.0f / (float)(y1 - y0);
    memset(&g, 0, sizeof(g));
    g.s = dsdx;
    /* colour, Z, fog and Q of the second vertex */
    base = a1;
    base.s = a0.s + dsdx * ((float)(px0 * 16 - x0) / 16.0f);
    if (py0 < d->scy0)
        py0 = d->scy0;
    if (py1 > d->scy1 + 1)
        py1 = d->scy1 + 1;
    if (py0 < band->ylo)
        py0 = band->ylo;
    if (py1 > band->yhi)
        py1 = band->yhi;
    for (y = band_next(band, py0, &run_end); y < py1; y++) {
        if (y >= run_end) {
            y = band_next(band, y, &run_end);
            if (y >= py1)
                break;
        }
        base.t = a0.t + dtdy * ((float)(y * 16 - y0) / 16.0f);
        span(d, y, px0, px1, base, &g);
    }
}
/* A sprite as draw_sprite and span set it up: the attributes at the
   first pixel of a row, the columns and rows it draws */
typedef struct SpritePrep {
    Attr a0, base, g;
    int32_t y0;
    float dtdy;
    int px0, px1, py0, py1;
} SpritePrep;

static int sprite_prep(const GsDraw *d, const GsVertex *v0, const GsVertex *v1, SpritePrep *sp) {
    Attr a1;
    int32_t x0 = v0->x, x1 = v1->x, y0 = v0->y, y1 = v1->y;
    float dsdx;

    vert_attr(d, &sp->a0, v0);
    vert_attr(d, &a1, v1);
    if (x0 > x1) {
        int32_t tx = x0;
        float ts = sp->a0.s;
        x0 = x1;
        x1 = tx;
        sp->a0.s = a1.s;
        a1.s = ts;
    }
    if (y0 > y1) {
        int32_t ty = y0;
        float tt = sp->a0.t;
        y0 = y1;
        y1 = ty;
        sp->a0.t = a1.t;
        a1.t = tt;
    }
    if (x0 == x1 || y0 == y1)
        return 0;
    sp->px0 = (int)ceil_div(x0, 16);
    sp->px1 = (int)ceil_div(x1, 16);
    sp->py0 = (int)ceil_div(y0, 16);
    sp->py1 = (int)ceil_div(y1, 16);
    dsdx = (a1.s - sp->a0.s) * 16.0f / (float)(x1 - x0);
    sp->dtdy = (a1.t - sp->a0.t) * 16.0f / (float)(y1 - y0);
    sp->y0 = y0;
    memset(&sp->g, 0, sizeof(sp->g));
    sp->g.s = dsdx;
    sp->base = a1;
    sp->base.s = sp->a0.s + dsdx * ((float)(sp->px0 * 16 - x0) / 16.0f);
    if (sp->py0 < d->scy0)
        sp->py0 = d->scy0;
    if (sp->py1 > d->scy1 + 1)
        sp->py1 = d->scy1 + 1;
    /* the columns, as span clips them */
    if (sp->px0 < d->scx0) {
        attr_step(&sp->base, &sp->g, (float)(d->scx0 - sp->px0));
        sp->px0 = d->scx0;
    }
    if (sp->px1 > d->scx1 + 1)
        sp->px1 = d->scx1 + 1;
    return sp->px0 < sp->px1 && sp->py0 < sp->py1;
}

/* The lowest and highest texels coordinates c0..c1 (1/256 texel) read,
   nearest (c >> 8) or bilinear ((c - 128) >> 8 and the next if it has
   weight) */
static void texel_span(int32_t c0, int32_t c1, int32_t *lo, int32_t *hi) {
    int32_t mn = c0 < c1 ? c0 : c1, mx = c0 < c1 ? c1 : c0, h;
    *lo = (mn - 128) >> 8;
    h = ((mx - 128) >> 8) + (((mx - 128) & 255) != 0);
    *hi = (mx >> 8) > h ? (mx >> 8) : h;
}

/* A texture coordinate in 1/256 texel as span_impl computes it */
static int32_t coord_f(float st, float iq, float size) {
    float f = st * iq * size;
    f = f < 1.0e6f ? (f > -1.0e6f ? f : -1.0e6f) : 1.0e6f;
    return ffloor(f * 256.0f);
}

static int32_t sprite_coord_t(const GsDraw *d, const SpritePrep *sp, int y) {
    float t = sp->a0.t + sp->dtdy * ((float)(y * 16 - sp->y0) / 16.0f);
    if (d->fst)
        return dfloor((double)t * 65536.0) >> 8;
    return coord_f(t, 1.0f / sp->base.q, (float)(1 << d->t.th));
}

/* The texel coordinates a sprite reads, before wrapping, computed as
   draw_sprite, span and span_impl compute them, at the first and last
   pixels and rows (in between they change monotonically). r: u0, u1, v0,
   v1 (inclusive), then the rows it draws, py0 and py1 (exclusive).
   Returns 0 if it draws nothing. */
int gs_sprite_texels(const GsDraw *d, const GsVertex *v0, const GsVertex *v1, int32_t r[6]) {
    SpritePrep sp;
    int n, i;

    if (!sprite_prep(d, v0, v1, &sp))
        return 0;
    n = sp.px1 - sp.px0;
    if (d->fst) {
        int32_t uf = dfloor((double)sp.base.s * 65536.0), du = dfloor((double)sp.g.s * 65536.0 + 0.5);
        int32_t ul = (int32_t)((uint32_t)uf + (uint32_t)du * (uint32_t)(n - 1));
        texel_span(uf >> 8, ul >> 8, &r[0], &r[1]);
    } else {
        float iq = 1.0f / sp.base.q, tw = (float)(1 << d->t.tw), s = sp.base.s;
        for (i = 1; i < n; i++)
            s += sp.g.s;
        texel_span(coord_f(sp.base.s, iq, tw), coord_f(s, iq, tw), &r[0], &r[1]);
    }
    texel_span(sprite_coord_t(d, &sp, sp.py0), sprite_coord_t(d, &sp, sp.py1 - 1), &r[2], &r[3]);
    r[4] = sp.py0;
    r[5] = sp.py1;
    return 1;
}

/* The texel rows (inclusive, before wrapping) row y of a sprite reads */
void gs_sprite_row_texels(const GsDraw *d, const GsVertex *v0, const GsVertex *v1, int y, int32_t r[2]) {
    SpritePrep sp;
    int32_t c;
    sprite_prep(d, v0, v1, &sp);
    c = sprite_coord_t(d, &sp, y);
    texel_span(c, c, &r[0], &r[1]);
}

static void draw_line(const GsDraw *d, const GsBand *band, const GsVertex *v0, const GsVertex *v1) {
    Attr a0, a1, g, a;
    int32_t dx = v1->x - v0->x, dy = v1->y - v0->y;
    int i, n0, n1;
    float *pa, *p0, *p1;

    vert_attr(d, &a0, v0);
    vert_attr(d, &a1, v1);
    memset(&g, 0, sizeof(g));
    if (!d->iip) {
        set_flat_colour(&a0, &g, v1);
        set_flat_colour(&a1, &g, v1);
    }
    if (dx == 0 && dy == 0)
        return;
    if ((dx < 0 ? -dx : dx) >= (dy < 0 ? -dy : dy)) {
        /* x-major: one pixel per column */
        int32_t xa = v0->x, xb = v1->x, ya = v0->y;
        int x;
        if (dx < 0) {
            Attr t = a0;
            a0 = a1;
            a1 = t;
            xa = v1->x;
            xb = v0->x;
            ya = v1->y;
            dx = -dx;
            dy = -dy;
        }
        n0 = (int)ceil_div(xa, 16);
        n1 = (int)ceil_div(xb, 16);
        for (x = n0; x < n1; x++) {
            float k = (float)(x * 16 - xa) / (float)dx;
            int y = (int)((ya + (int64_t)(x * 16 - xa) * dy / dx + 8) >> 4);
            if (!gs_band_owns(band, y))
                continue;
            a.z = a0.z + (a1.z - a0.z) * k;
            pa = &a.r; p0 = &a0.r; p1 = &a1.r;
            for (i = 0; i < 8; i++)
                pa[i] = p0[i] + (p1[i] - p0[i]) * k;
            span(d, y, x, x + 1, a, &g);
        }
    } else {
        int32_t ya = v0->y, yb = v1->y, xa = v0->x;
        int y;
        if (dy < 0) {
            Attr t = a0;
            a0 = a1;
            a1 = t;
            ya = v1->y;
            yb = v0->y;
            xa = v1->x;
            dx = -dx;
            dy = -dy;
        }
        n0 = (int)ceil_div(ya, 16);
        n1 = (int)ceil_div(yb, 16);
        for (y = n0; y < n1; y++) {
            float k = (float)(y * 16 - ya) / (float)dy;
            int x = (int)((xa + (int64_t)(y * 16 - ya) * dx / dy + 8) >> 4);
            if (!gs_band_owns(band, y))
                continue;
            a.z = a0.z + (a1.z - a0.z) * k;
            pa = &a.r; p0 = &a0.r; p1 = &a1.r;
            for (i = 0; i < 8; i++)
                pa[i] = p0[i] + (p1[i] - p0[i]) * k;
            span(d, y, x, x + 1, a, &g);
        }
    }
}

static void draw_point(const GsDraw *d, const GsBand *band, const GsVertex *v) {
    Attr a, g;
    int y = (v->y + 8) >> 4;
    if (!gs_band_owns(band, y))
        return;
    vert_attr(d, &a, v);
    memset(&g, 0, sizeof(g));
    span(d, y, (v->x + 8) >> 4, ((v->x + 8) >> 4) + 1, a, &g);
}

void gs_render(const GsDraw *d, const GsBand *band, const GsVertex *v, uint32_t type) {
    switch (type) {
    case 0: draw_point(d, band, &v[0]); break;
    case 1: case 2: draw_line(d, band, &v[0], &v[1]); break;
    case 3: case 4: case 5: draw_triangle(d, band, &v[0], &v[1], &v[2]); break;
    case 6: draw_sprite(d, band, &v[0], &v[1]); break;
    default: break;
    }
}
