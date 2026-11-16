/* Widescreen in the GS front end (port/docs/widescreen.md).

   At an aspect wider than 4:3 the game draws as before, in its 640-pixel
   screen space, but with its view culling widened (port/tools/fixups/
   widescreen.py), so the 3D scene goes on past x = 0 and x = 640. Here
   every primitive drawn into a 640-wide frame buffer is squeezed
   horizontally about the screen's centre by k = (4/3) / aspect, which
   brings that wider scene into the frame buffer; the presenter stretches
   the frame back to the aspect. 3D comes out Hor+, and 2D, squeezed the
   same way, keeps its proportions in a 4:3 area in the middle.

   Some primitives are about the frame itself, not things in it, and are
   left as they are (they then cover the whole wide frame):
   - those textured from pages that draws wrote (the frame copies, blurs,
     the per-frame copy of the last frame): their texture coordinates are
     frame pixels, which squeezing would no longer match;
   - untextured 2D that covers the screen's whole width (clears, fades,
     flashes), and full-width 2D whose texture is stretched over it rather
     than drawn texel for texel (a texture used as a colour);
   - untextured 2D of the screen's full height touching a side (wipes):
     the vertices on that side stay at the frame's edge.
   3D is squeezed: the VU1's output (PATH1) and the CPU clipper's packets,
   whose GIF tag the port marks (bit 44, port/game/main/45AC74.c). The
   exception is a screen-filling effect: a 3D packet that is a plane
   facing the camera (one depth) whose vertices, together, span the whole
   4:3 screen, drawn with blending other than plain alpha (additive, a
   fixed alpha). Such flashes are sized to cover the 4:3 screen with some
   margin, which squeezed would stop short of the sides; left as they
   are, they cover the frame, stretched with it. Scenery, drawn with plain
   alpha blending, stays squeezed. A scissor rectangle narrower than the
   screen is squeezed with the 2D it frames; a full-width one covers the
   whole frame.

   gs_ws_k is 1 at 4:3 and everything here is skipped (gs_ws_on 0): the
   output is exactly the 4:3 renderer's. */
#include <math.h>
#include "gs_internal.h"
#include "gs_draw.h"

int gs_ws_on;
float gs_ws_k = 1.0f;
float gs_ws_aspect = 4.0f / 3.0f;
int gs_ws_3d;
int gs_ws_fill;
int gs_ws_squeezed;

/* Who last wrote each page: 0 not seen, 1 a transfer, 2 a draw (or a
   frame captured to main memory and uploaded again) */
static uint8_t page_src[512];

#define X_MID (320 * 16)
#define X_END (640 * 16)

void gs_ws_set(int on, float aspect) {
    float k;
    if (aspect < 4.0f / 3.0f)
        aspect = 4.0f / 3.0f;
    k = on ? (4.0f / 3.0f) / aspect : 1.0f;
    if (k > 0.9995f)
        k = 1.0f;
    if (on && !gs_ws_on)
        memset(page_src, 0, sizeof(page_src));
    if (k != gs_ws_k)
        gs.dirty = 1; /* the scissor */
    gs_ws_on = on;
    gs_ws_k = k;
    gs_ws_aspect = k == 1.0f ? 4.0f / 3.0f : aspect;
}

static inline int32_t squeeze(int32_t x) {
    return X_MID + (int32_t)lrintf((float)(x - X_MID) * gs_ws_k);
}

/* x0 and x1 are 11-bit two's complement here: the HUD the port keeps at
   the screen's left edge (the field's mini-map, port/tools/fixups/
   widescreen.py) is framed by a scissor that starts left of x = 0. A
   640-wide frame never has one past 1023 otherwise. */
uint64_t gs_ws_scissor_wide(uint64_t sc) {
    int32_t x0 = (int32_t)(GS_FIELD(sc, 0, 11) ^ 0x400) - 0x400, x1 = (int32_t)(GS_FIELD(sc, 16, 11) ^ 0x400) - 0x400;
    if (x0 <= 0 && x1 >= 639)
        return sc;
    /* The pixels whose centres the squeezed rectangle covers */
    x0 = (squeeze(x0 * 16) + 15) >> 4;
    x1 = ((squeeze((x1 + 1) * 16) + 15) >> 4) - 1;
    if (x0 < 0)
        x0 = 0;
    if (x1 < x0)
        x1 = x0;
    return (sc & ~((uint64_t)0x7FF | (uint64_t)0x7FF << 16)) | (uint64_t)x0 | (uint64_t)x1 << 16;
}

void gs_ws_uploaded(const GsPages *p) {
    int i;
    if (!gs_ws_on)
        return;
    for (i = 0; i < 512; i++)
        if ((p->w[i >> 5] >> (i & 31)) & 1)
            page_src[i] = 1;
}

/* Frames read back to main memory (GphFbStoreImage, at the game's
   sysScreenCaptureReq) and uploaded again later: the title menu's Continue
   and Configurations backgrounds (GphFbOverlayImage), the battle start's
   shatter, the field and battle screen effects. The picture is the wide
   frame already, so its uploads count as drawn frame pages: the 2D that
   draws them back is left as it is, like a frame copy. A transfer's data
   comes straight from the game's memory (DMA passes its pointers), so
   the upload is told by where its data is. */
#define NCAPS 4
static struct {
    uintptr_t lo, hi;
} caps[NCAPS];
static int ncap_next;

void gs_ws_read_back(const void *dst, uint32_t bytes, const GsPages *p) {
    uintptr_t lo = (uintptr_t)dst, hi = lo + bytes;
    int i, drawn = 0;
    if (!gs_ws_on)
        return;
    for (i = 0; i < 512 && !drawn; i++)
        drawn = ((p->w[i >> 5] >> (i & 31)) & 1) && page_src[i] == 2;
    for (i = 0; i < NCAPS; i++) {
        /* A strip that continues a capture extends it; one over it ends it */
        if (caps[i].hi == lo && drawn) {
            caps[i].hi = hi;
            return;
        }
        if (lo < caps[i].hi && hi > caps[i].lo)
            caps[i].lo = caps[i].hi = 0;
    }
    if (drawn) {
        caps[ncap_next].lo = lo;
        caps[ncap_next].hi = hi;
        ncap_next = (ncap_next + 1) % NCAPS;
    }
}

void gs_ws_upload_data(const void *data, const GsPages *p) {
    uintptr_t a = (uintptr_t)data;
    int i;
    if (!gs_ws_on)
        return;
    for (i = 0; i < NCAPS; i++)
        if (a >= caps[i].lo && a < caps[i].hi)
            break;
    if (i == NCAPS)
        return;
    for (i = 0; i < 512; i++)
        if ((p->w[i >> 5] >> (i & 31)) & 1)
            page_src[i] = 2;
}

/* Whether a 3D packet is a plane facing the camera that spans the whole
   4:3 screen (0..640, 0..448): its vertices, together, cover it, all at
   the same depth (within 1/64). Its PACKED data from the tag on, as far
   as this call has it. Its primitives are each smaller (a flash is tens
   of triangles), so this is decided for the packet as a whole, before it
   is drawn. The depth keeps out effects that are objects in the scene
   around the camera, such as a cone of light over a character: those
   have to stay squeezed with the scene. */
int gs_ws_tag_fills(uint64_t lo, uint64_t regs, const uint8_t *p, uint32_t qwc) {
    uint64_t prim = GS_FIELD(lo, 46, 1) ? GS_FIELD(lo, 47, 11) : gs.prim;
    uint64_t attr = (gs.prmodecont & 1) ? prim : gs.prmode;
    const GsContext *c = &gs.ctx[GS_FIELD(attr, 9, 1)];
    int32_t ox = (int32_t)GS_FIELD(c->xyoffset, 0, 16), oy = (int32_t)GS_FIELD(c->xyoffset, 32, 16);
    int32_t x0 = INT32_MAX, x1 = INT32_MIN, y0 = INT32_MAX, y1 = INT32_MIN;
    uint32_t nreg = GS_FIELD(lo, 60, 4), n, i, z0 = UINT32_MAX, z1 = 0;

    if (!nreg)
        nreg = 16;
    n = (uint32_t)(lo & 0x7FFF) * nreg;
    if (n > qwc)
        n = qwc;
    for (i = 0; i < n; i++) {
        uint32_t r = (uint32_t)(regs >> (i % nreg * 4)) & 15, w[3], z;
        int32_t x, y;
        if (r != 0x4 && r != 0x5 && r != 0xC && r != 0xD) /* XYZF2, XYZ2, XYZF3, XYZ3 */
            continue;
        memcpy(w, p + (size_t)i * 16, 12);
        x = (int32_t)(w[0] & 0xFFFF) - ox;
        y = (int32_t)(w[1] & 0xFFFF) - oy;
        z = (r & 1) ? w[2] : (w[2] >> 4) & 0xFFFFFF;
        if (x < x0) x0 = x;
        if (x > x1) x1 = x;
        if (y < y0) y0 = y;
        if (y > y1) y1 = y;
        if (z < z0) z0 = z;
        if (z > z1) z1 = z;
    }
    return x0 <= 0 && x1 >= X_END && y0 <= 0 && y1 >= 448 * 16 && z1 - z0 <= z1 / 64;
}

/* The pages of a 640-wide frame (CT32/24 or CT16) a box of pixels covers */
static void mark_drawn(uint64_t frame, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint64_t sc) {
    uint32_t fbp = GS_FIELD(frame, 0, 9), fbw = GS_FIELD(frame, 16, 6), psm = GS_FIELD(frame, 24, 6);
    int ph = psm == GS_PSMCT32 || psm == GS_PSMCT24 ? 32 : 64, px, py;
    int32_t sx0 = (int32_t)GS_FIELD(sc, 0, 11), sx1 = (int32_t)GS_FIELD(sc, 16, 11);
    int32_t sy0 = (int32_t)GS_FIELD(sc, 32, 11), sy1 = (int32_t)GS_FIELD(sc, 48, 11);
    x0 >>= 4;
    y0 >>= 4;
    x1 = (x1 + 15) >> 4;
    y1 = (y1 + 15) >> 4;
    if (x0 < sx0) x0 = sx0;
    if (y0 < sy0) y0 = sy0;
    if (x1 > sx1) x1 = sx1;
    if (y1 > sy1) y1 = sy1;
    if (x0 > x1 || y0 > y1)
        return;
    for (py = y0 / ph; py <= y1 / ph; py++)
        for (px = x0 / 64; px <= x1 / 64; px++)
            page_src[(fbp + (uint32_t)py * fbw + (uint32_t)px) & 511] = 2;
}

const GsVertex *gs_ws_prim(const GsVertex *v, uint32_t type, GsVertex *tmp) {
    static const int nvs[8] = {1, 2, 2, 3, 3, 3, 2, 0};
    uint64_t attr = (gs.prmodecont & 1) ? gs.prim : gs.prmode;
    const GsContext *c = &gs.ctx[GS_FIELD(attr, 9, 1)];
    uint32_t fpsm = GS_FIELD(c->frame, 24, 6);
    int nv = nvs[type & 7], i, tme, mode = 0;
    int32_t x0, x1, y0, y1;
    const GsVertex *out = v;

    gs_ws_squeezed = 0;
    if (GS_FIELD(c->frame, 16, 6) != 10 || nv == 0 ||
        !(fpsm == GS_PSMCT32 || fpsm == GS_PSMCT24 || fpsm == GS_PSMCT16 || fpsm == GS_PSMCT16S))
        return v;
    x0 = x1 = v[0].x;
    y0 = y1 = v[0].y;
    for (i = 1; i < nv; i++) {
        if (v[i].x < x0) x0 = v[i].x;
        if (v[i].x > x1) x1 = v[i].x;
        if (v[i].y < y0) y0 = v[i].y;
        if (v[i].y > y1) y1 = v[i].y;
    }
    if (gs_ws_k == 1.0f) {
        mark_drawn(c->frame, x0, y0, x1, y1, c->scissor);
        return v;
    }

    /* mode 0: squeeze; 1: leave as it is; 2: squeeze but keep the vertices
       on or past a side where they are */
    tme = (int)GS_FIELD(attr, 4, 1);
    if (!gs_ws_3d) {
        int full_w = x0 <= 8 && x1 >= X_END - 8;
        if (tme) {
            uint32_t tbp = GS_FIELD(c->tex0, 0, 14);
            if (page_src[(tbp >> 5) & 511] == 2) {
                mode = 1;
            } else if (full_w) {
                /* The texture's span across the screen, in texels */
                float u0 = 1e30f, u1 = -1e30f;
                int fst = (int)GS_FIELD(attr, 8, 1);
                float tw = (float)(1u << (GS_FIELD(c->tex0, 26, 4) > 10 ? 10 : GS_FIELD(c->tex0, 26, 4)));
                for (i = 0; i < nv; i++) {
                    float u = fst ? (float)v[i].u / 16.0f : (v[i].q != 0.0f ? v[i].s / v[i].q * tw : 0.0f);
                    if (u < u0) u0 = u;
                    if (u > u1) u1 = u;
                }
                if (u1 - u0 < (float)(x1 - x0) / 32.0f)
                    mode = 1;
            }
        } else if (full_w) {
            mode = 1;
        } else if (y0 <= 8 && y1 >= 448 * 16 - 8 && (x0 <= 8 || x1 >= X_END - 8)) {
            mode = 2;
        }
    } else if (gs_ws_fill && GS_FIELD(attr, 6, 1) && (c->alpha & 0xFF) != 0x44) {
        /* A screen-filling effect (ABE, blending other than
           (Cs - Cd) * As + Cd) */
        mode = 1;
    }
    if (mode != 1) {
        for (i = 0; i < nv; i++) {
            tmp[i] = v[i];
            if (mode == 2 && (v[i].x <= 8 || v[i].x >= X_END - 8))
                continue;
            tmp[i].x = squeeze(v[i].x);
        }
        out = tmp;
        gs_ws_squeezed = 1;
        x0 = squeeze(x0);
        x1 = squeeze(x1);
    }
    mark_drawn(c->frame, x0, y0, x1, y1, gs_ws_scissor(c->scissor));
    return out;
}

/* After a save state's load: what the heuristics learned of pages and
   read-back buffers belongs to the frames before it */
void gs_ws_forget(void) {
    memset(page_src, 0, sizeof(page_src));
    memset(caps, 0, sizeof(caps));
    ncap_next = 0;
}
