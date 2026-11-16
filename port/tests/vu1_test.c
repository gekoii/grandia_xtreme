/* Test of the VU1 microprogram in C (port/src/vu1/): synthetic meshes laid
   out in VU1 memory as the VIF would unpack them, run through each
   variant's phases, and the kicked GIF packets checked against values
   worked out by hand from the microcode's arithmetic (port/docs/vu.md,
   build/port/vu/vuMicroCode1.txt).

   It also checks the VU0 kernels (port/src/vu0_micro.c) where they depend
   on the pipeline timing: Q 7 pairs after DIV (VuProjectVerts' three-call
   pipeline), the MAC flags 4 pairs after the operation (the triangle facing
   tests) and the clip flags 4 pairs after CLIP (the billboard kernel's
   FCGETs).

   Build and run (from the repository root; any output directory):
     i686-w64-mingw32-clang -std=gnu11 -Wall -msse2 -mfpmath=sse -Iport/include \
         port/tests/vu1_test.c port/src/vu1/vu1.c port/src/vu1/vu1_vertex.c \
         port/src/vu1/vu1_tri.c port/src/vu0_micro.c port/src/ee.c -o vu1_test.exe
     ./vu1_test.exe */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_vu1.h"
#include "grx_gs.h"
#include "grx_ee.h"
#include "grx_vu0_micro.h"

uint8_t grx_vu1_data[GRX_VU1_DATA_SIZE] __attribute__((aligned(16)));
uint8_t grx_vu1_micro[GRX_VU1_MICRO_SIZE] __attribute__((aligned(16)));

static int failures, checks;

#define CHECK(cond, ...)                                \
    do {                                                \
        checks++;                                       \
        if (!(cond)) {                                  \
            failures++;                                 \
            printf("FAIL %s:%d: ", __FILE__, __LINE__); \
            printf(__VA_ARGS__);                        \
            printf("\n");                               \
        }                                               \
    } while (0)

/* ---- what the program calls ---- */

static uint32_t gif[1024][4];
static uint32_t gif_qwc;
static int gif_calls, gif_path;
static int logs;

void grx_gs_gif(const void *data, uint32_t qwc, int path) {
    gif_calls++;
    gif_path = path;
    gif_qwc = qwc;
    memcpy(gif, data, qwc * 16 > sizeof gif ? sizeof gif : qwc * 16);
}

void grx_log(const char *fmt, ...) {
    va_list ap;
    logs++;
    va_start(ap, fmt);
    printf("  log: ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    exit(1);
}

/* VU0 memory isn't mapped in the test: vu0_micro.c uses its own buffer */
int grx_hwmem_mapped(uint32_t addr, uint32_t size) {
    (void)addr, (void)size;
    return 0;
}

/* ---- VU memory ---- */

typedef union {
    float f[4];
    int32_t i[4];
    uint32_t u[4];
} Q;

static Q *mem(uint32_t qa) {
    return (Q *)(grx_vu1_data + qa * 16);
}

static void setf(uint32_t qa, float x, float y, float z, float w) {
    Q *q = mem(qa);
    q->f[0] = x, q->f[1] = y, q->f[2] = z, q->f[3] = w;
}

static void seti(uint32_t qa, int32_t x, int32_t y, int32_t z, int32_t w) {
    Q *q = mem(qa);
    q->i[0] = x, q->i[1] = y, q->i[2] = z, q->i[3] = w;
}

static void setw_f(uint32_t qa, float w) {
    mem(qa)->f[3] = w;
}

/* A fresh VU: memory cleared, the init program run as the microcode
   block's own VIF stream does (UNPACK (0,0,0,0), (0,0,255,0) to 0; MSCAL 0) */
static void reset_vu(void) {
    memset(grx_vu1_data, 0, sizeof grx_vu1_data);
    setf(1, 0, 0, 255, 0);
    grx_vu1_run(0);
    memset(grx_vu1_data, 0, sizeof grx_vu1_data);
    gif_calls = 0;
    gif_qwc = 0;
}

/* The GIF tag: NLOOP triangles, EOP, PRE, PRIM = triangle, PACKED, 9
   registers (ST, RGBAQ, XYZF2) x 3; mode byte in bits 32-39 */
static void gif_tag(uint32_t qa, uint32_t ntri, uint32_t mode) {
    Q *q = mem(qa);
    q->u[0] = ntri | 0x8000u;
    q->u[1] = mode | (1u << 14) | (3u << 15) | (9u << 28);
    q->u[2] = 0x12412412u;
    q->u[3] = 0x4u;
}

typedef struct Hdr {
    float proj[4], off[4];
    uint32_t first, count, ntri;
    float dist;
    uint32_t mode;
    float amb[4];
    float lmtx[3][4];
    float mv[4][4];
    float fog[4], uvoff[4];
} Hdr;

static void put_header(const Hdr *h, int vu1_layout) {
    int i;
    setf(0x1A0, h->proj[0], h->proj[1], h->proj[2], h->proj[3]);
    setf(0x1A1, h->off[0], h->off[1], h->off[2], h->off[3]);
    seti(0x1A2, (int32_t)h->first, (int32_t)h->count, (int32_t)h->ntri, 0);
    setw_f(0x1A2, h->dist);
    gif_tag(0x1A3, h->ntri, h->mode);
    setf(0x1A4, h->amb[0], h->amb[1], h->amb[2], h->amb[3]);
    for (i = 0; i < 3; i++) {
        setf(0x1A5 + i, h->lmtx[i][0], h->lmtx[i][1], h->lmtx[i][2], h->lmtx[i][3]);
    }
    if (vu1_layout) {
        for (i = 0; i < 4; i++) {
            setf(0x1A8 + i, h->mv[i][0], h->mv[i][1], h->mv[i][2], h->mv[i][3]);
        }
        setf(0x1AC, h->fog[0], h->fog[1], h->fog[2], h->fog[3]);
        setf(0x1AD, h->uvoff[0], h->uvoff[1], h->uvoff[2], h->uvoff[3]);
    } else {
        setf(0x1A8, h->fog[0], h->fog[1], h->fog[2], h->fog[3]);
        setf(0x1A9, h->uvoff[0], h->uvoff[1], h->uvoff[2], h->uvoff[3]);
    }
}

/* The test scene: 3 vertices, screen centre (320, 224), scale 100, the
   model-view a translation by z + 2, distance 2. With p the view-space
   position and q = 2 / p.z (vu.md, "Project"):
     X = (2048 + 320) 16 + 1600 x q,  Y = (2048 + 224) 16 - 1600 y q,  Z = 16 q
     F = 16 (0 + 2) + 64 / (4 + |p|^2)
   v0 (0,0,2) -> p (0,0,4), q 0.5: X 37888, Y 36352, Z 8,  F 32 + 64/20 = 35.2 -> 35
   v1 (1,0,2) -> p (1,0,4), q 0.5: X 38688, Y 36352, Z 8,  F 32 + 64/21 = 35.04 -> 35
   v2 (0,1,0) -> p (0,1,2), q 1:   X 37888, Y 34752, Z 16, F 32 + 64/9  = 39.1 -> 39
   UVs (4.12, z = 1.0 from STROW) times 4, plus (0.5, 0.25), times q:
   v0 (4096, 2048) -> (4.5, 2.25) q = (2.25, 1.125, 0.5)
   v1 (0, 0)       -> (0.5, 0.25) q = (0.25, 0.125, 0.5)
   v2 (-4096,1024) -> (-3.5, 1.25) q = (-3.5, 1.25, 1)
   Colours + ambient (10, 20, 30, 0), clamped:
   v0 (100,150,200,128) -> (110,170,230,128); v1 (250,240,0,255) -> (255,255,30,255);
   v2 (0,0,0,0) -> (10,20,30,0) */
static const int32_t exp_xyzf[3][4] = {{37888, 36352, 8, 35}, {38688, 36352, 8, 35}, {37888, 34752, 16, 39}};
static const float exp_stq[3][3] = {{2.25f, 1.125f, 0.5f}, {0.25f, 0.125f, 0.5f}, {-3.5f, 1.25f, 1.0f}};
static const int32_t exp_rgba[3][4] = {{110, 170, 230, 128}, {255, 255, 30, 255}, {10, 20, 30, 0}};

static Hdr scene_header(uint32_t ntri, uint32_t mode) {
    Hdr h;
    memset(&h, 0, sizeof h);
    h.proj[0] = 1600, h.proj[1] = -1600, h.proj[2] = 16;
    h.off[0] = (2048 + 320) * 16, h.off[1] = (2048 + 224) * 16;
    h.count = 3, h.ntri = ntri, h.dist = 2;
    h.mode = mode;
    h.amb[0] = 10, h.amb[1] = 20, h.amb[2] = 30;
    h.lmtx[0][0] = h.lmtx[1][1] = h.lmtx[2][2] = 1;
    h.mv[0][0] = h.mv[1][1] = h.mv[2][2] = h.mv[3][3] = 1;
    h.mv[3][2] = 2;
    h.fog[0] = 2, h.fog[1] = 64, h.fog[2] = 255, h.fog[3] = 4;
    h.uvoff[0] = 0.5f, h.uvoff[1] = 0.25f;
    return h;
}

/* Model-space positions (VU1 paths), w = the vertex's index */
static void scene_positions(int view_space) {
    float dz = view_space ? 2 : 0;
    setf(0, 0, 0, 2 + dz, 0);
    setf(1, 1, 0, 2 + dz, 0);
    setf(2, 0, 1, 0 + dz, 0);
    mem(0)->i[3] = 0, mem(1)->i[3] = 1, mem(2)->i[3] = 2;
}

static void scene_colours(void) {
    seti(0x100, 100, 150, 200, 128);
    seti(0x101, 250, 240, 0, 255);
    seti(0x102, 0, 0, 0, 0);
}

static void scene_uvs(void) {
    seti(0x80, 4096, 2048, 0, 0);
    seti(0x81, 0, 0, 0, 0);
    seti(0x82, -4096, 1024, 0, 0);
    setw_f(0x80, 0), setw_f(0x81, 0), setw_f(0x82, 0);
    mem(0x80)->f[2] = mem(0x81)->f[2] = mem(0x82)->f[2] = 1.0f;
}

/* Triangles: VU1 paths send indices times 16 */
static void put_tri(uint32_t t, uint32_t a, uint32_t b, uint32_t c, int times16) {
    uint32_t s = times16 ? 16 : 1;
    seti(0x17F + t, (int32_t)(a * s), (int32_t)(b * s), (int32_t)(c * s), 0);
}

/* Checks the packet's triangle t against the expected vertices */
static void check_vertex(const char *what, uint32_t t, int k, int v) {
    const uint32_t *st = gif[1 + 9 * t + 3 * k], *rgba = gif[2 + 9 * t + 3 * k], *xyzf = gif[3 + 9 * t + 3 * k];
    float s[3];
    int i;
    memcpy(s, st, 12);
    for (i = 0; i < 3; i++) {
        CHECK(s[i] == exp_stq[v][i], "%s: tri %u vertex %d (v%d) STQ[%d] = %g, expected %g", what, t, k, v, i,
              s[i], exp_stq[v][i]);
    }
    for (i = 0; i < 4; i++) {
        CHECK((int32_t)rgba[i] == exp_rgba[v][i], "%s: tri %u v%d RGBA[%d] = %d, expected %d", what, t, v, i,
              (int32_t)rgba[i], exp_rgba[v][i]);
    }
    for (i = 0; i < 3; i++) {
        CHECK((int32_t)xyzf[i] == exp_xyzf[v][i], "%s: tri %u v%d XYZF[%d] = %d, expected %d", what, t, v, i,
              (int32_t)xyzf[i], exp_xyzf[v][i]);
    }
}

static int32_t xyzf_w(uint32_t t, int k) {
    return (int32_t)gif[3 + 9 * t + 3 * k][3];
}

static void check_tag(const char *what, uint32_t ntri) {
    CHECK(gif_calls == 1, "%s: %d kicks, expected 1", what, gif_calls);
    CHECK(gif_path == GRX_GIF_PATH1, "%s: path %d", what, gif_path);
    CHECK(gif_qwc == 1 + 9 * ntri, "%s: kicked %u qw, expected %u", what, gif_qwc, 1 + 9 * ntri);
    CHECK((gif[0][0] & 0x7FFF) == ntri && (gif[0][0] & 0x8000), "%s: GIF tag 0x%08X", what, gif[0][0]);
}

/* ---- VU1 unlit ---- */

static void run_vu1_unlit(uint32_t mode_entry, uint32_t ntri, uint32_t mode_byte, int extra_phase) {
    Hdr h = scene_header(ntri, mode_byte);
    put_header(&h, 1);
    scene_colours();
    scene_positions(0);
    grx_vu1_run(mode_entry); /* phase 1 */
    CHECK(gif_calls == 0, "unlit: kick in phase 1");
    scene_uvs(); /* the VIF: UVs, FLUSHE, MSCNT, triangles (see the caller) */
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 2: project (+ keys) */
    CHECK(gif_calls == 0, "unlit: kick in phase 2");
    if (extra_phase) {
        CHECK(gif_calls == 0, "unlit: kick before the sort");
    }
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 3: (sort,) assemble, kick */
}

static void test_vu1_unlit_nocull(void) {
    reset_vu();
    put_tri(0, 0, 1, 2, 1);
    run_vu1_unlit(0xF0 / 8, 1, 0, 0); /* mode 2 */
    check_tag("unlit mode 2", 1);
    check_vertex("unlit mode 2", 0, 0, 0);
    check_vertex("unlit mode 2", 0, 1, 1);
    check_vertex("unlit mode 2", 0, 2, 2);
    CHECK(xyzf_w(0, 0) == 35 && xyzf_w(0, 1) == 35 && xyzf_w(0, 2) == 39, "unlit: F = %d %d %d, expected 35 35 39",
          xyzf_w(0, 0), xyzf_w(0, 1), xyzf_w(0, 2));
    /* the GIF tag is the header's, at 0x1BF */
    CHECK(memcmp(gif[0], mem(0x1A3), 16) == 0, "unlit: tag differs from the header's");
    /* phase 1 converts and transforms (count + 3) & ~3 = 4 slots */
    CHECK(mem(3)->f[2] == 2.0f, "unlit: slot 3 not transformed (z = %g)", mem(3)->f[2]);
}

/* Mode 0 culls a negative screen cross product, mode 1 a non-negative one.
   Screen (pixels): v0 (2368, 2272), v1 (2418, 2272), v2 (2368, 2172).
   (0,1,2): e1 = (50, 0), e2 = (0, -100), z = 50 (-100) - 0 = -5000 < 0
   (0,2,1): z = +5000. A culled triangle's last XYZF2 w is 0x8000 (ADC). */
static void test_vu1_cull(void) {
    int mode;
    for (mode = 0; mode < 2; mode++) {
        char what[32];
        snprintf(what, sizeof what, "cull mode %d", mode);
        reset_vu();
        put_tri(0, 0, 1, 2, 1);
        put_tri(1, 0, 2, 1, 1);
        run_vu1_unlit((0xD0 + 0x10 * mode) / 8, 2, 0, 0);
        check_tag(what, 2);
        check_vertex(what, 0, 0, 0);
        check_vertex(what, 0, 1, 1);
        check_vertex(what, 1, 1, 2);
        CHECK(xyzf_w(0, 2) == (mode == 0 ? 0x8000 : 39), "%s: tri 0 last w = 0x%X", what, xyzf_w(0, 2));
        CHECK(xyzf_w(1, 2) == (mode == 0 ? 35 : 0x8000), "%s: tri 1 last w = 0x%X", what, xyzf_w(1, 2));
        CHECK(xyzf_w(0, 0) == 35 && xyzf_w(1, 0) == 35, "%s: first vertices' w changed", what);
    }
    /* mode 1 also culls a zero-area triangle, mode 0 keeps it */
    for (mode = 0; mode < 2; mode++) {
        reset_vu();
        put_tri(0, 0, 0, 1, 1);
        run_vu1_unlit((0xD0 + 0x10 * mode) / 8, 1, 0, 0);
        CHECK(xyzf_w(0, 2) == (mode == 0 ? 35 : 0x8000), "cull mode %d: degenerate w = 0x%X", mode, xyzf_w(0, 2));
    }
}

/* Depth sort: a mesh whose mode byte has bit 6. The key is Z0 + Z1 + Z2
   (Z = 16 q = 32 / p.z here); the list comes out sorted ascending, equal
   keys in their order, and needs no extra MSCNT (the key pass ends the
   projection phase, the next MSCNT sorts and assembles). */
static uint32_t lcg = 12345;
static uint32_t rnd(uint32_t n) {
    lcg = lcg * 1103515245u + 12345u;
    return (lcg >> 16) % n;
}

static void test_vu1_sort(int ntri) {
    static const float zs[] = {1, 2, 4, 8, 16, 32, 0.5f, 3, 5};
    int nv = 9, i, t, sorted = 1, stable = 1;
    int32_t z[9];
    uint32_t tri[64][3];
    float prev_key = -1e30f;
    int prev_orig = -1;
    Hdr h;

    reset_vu();
    h = scene_header((uint32_t)ntri, 0x40);
    h.count = (uint32_t)nv;
    put_header(&h, 1);
    for (i = 0; i < nv; i++) {
        setf((uint32_t)i, (float)i, 0, zs[i] - 2, 0);
        mem((uint32_t)i)->i[3] = i;
        seti(0x100 + (uint32_t)i, i, 0, 0, 0);
    }
    grx_vu1_run(0xF0 / 8);
    for (i = 0; i < nv; i++) {
        seti(0x80 + (uint32_t)i, 0, 0, 0, 0);
        mem(0x80 + (uint32_t)i)->f[2] = 1.0f;
    }
    /* the triangles come after the MSCNT, over the light block and the
       header (the VIF runs the phase at its next wait, after them) */
    for (t = 0; t < ntri; t++) {
        for (i = 0; i < 3; i++) {
            tri[t][i] = rnd((uint32_t)nv);
        }
        put_tri((uint32_t)t, tri[t][0], tri[t][1], tri[t][2], 1);
    }
    grx_vu1_run(GRX_VU1_CONTINUE); /* project + keys */
    CHECK(gif_calls == 0, "sort: kicked early");
    for (i = 0; i < nv; i++) {
        z[i] = mem((uint32_t)i)->i[2];
        CHECK(z[i] == (int32_t)(32.0f / zs[i]), "sort: v%d Z = %d, expected %d", i, z[i], (int32_t)(32.0f / zs[i]));
    }
    grx_vu1_run(GRX_VU1_CONTINUE); /* sort + assemble */
    check_tag("sort", (uint32_t)ntri);
    /* identify each packet triangle by its vertices' colours (R = index) and
       its original position by matching in order of appearance */
    {
        int used[64] = {0};
        for (t = 0; t < ntri; t++) {
            /* R = index + the ambient's 10 */
            uint32_t a = gif[2 + 9 * t][0] - 10, b = gif[5 + 9 * t][0] - 10, c = gif[8 + 9 * t][0] - 10;
            float key = (float)z[a] + (float)z[b] + (float)z[c];
            int orig = -1, o;
            for (o = 0; o < ntri; o++) {
                if (!used[o] && tri[o][0] == a && tri[o][1] == b && tri[o][2] == c) {
                    orig = o;
                    break;
                }
            }
            CHECK(orig >= 0, "sort: triangle %d (%u %u %u) not in the input", t, a, b, c);
            if (orig >= 0) {
                used[orig] = 1;
            }
            if (key < prev_key) {
                sorted = 0;
            }
            if (key == prev_key && orig < prev_orig) {
                stable = 0;
            }
            prev_key = key;
            prev_orig = orig;
            /* the key is in the list's w */
            CHECK(mem(0x17F + (uint32_t)t)->f[3] == key, "sort: list w %g, key %g", mem(0x17F + (uint32_t)t)->f[3], key);
        }
    }
    CHECK(sorted, "sort (%d triangles): not ascending", ntri);
    CHECK(stable, "sort (%d triangles): equal keys reordered", ntri);
}

/* One triangle with the sort flag: the key pass is skipped (vi10 = 0) and
   the program ends where the unsorted one does */
static void test_vu1_sort_one(void) {
    reset_vu();
    put_tri(0, 0, 1, 2, 1);
    run_vu1_unlit(0xF0 / 8, 1, 0x40, 0);
    check_tag("sort, 1 triangle", 1);
    check_vertex("sort, 1 triangle", 0, 2, 2);
    CHECK(mem(0x17F)->i[3] == 0, "sort, 1 triangle: key written");
}

/* ---- VU1 lit ----
   Normal (0, 0, -1) (4.12: -4096), light matrix identity.
   Directional light 0: L (0, 0, -1), I 1, colour (100, 50, 25, 0):
     max(0, (1 0) 0 + (1 0) 0 + (1 -1)(-1)) = 1
   Point light 0 at the eye (0, 0, 0), range 8 (1/range 0.125), colour
   (40, 80, 120, 10). Vertex v0 at (0, 0, 4): d = (0, 0, -4) / 8, |d|^2 = 0.25,
   N.d = 0.5, 1/|d| = 2: -0.5 + 0.5 x 2 = 0.5 = (N.dhat)(1 - 4/8).
   Colour (1, 2, 3, 4) + 100,50,25,0 + 20,40,60,5 = (121, 92, 88, 9),
   + ambient (10, 20, 30, 0) = (131, 112, 118, 9).
   With the point lights off (all 1/range = 1): (111, 72, 58, 4). */
static void lit_lights(int point_on) {
    int i;
    for (i = 0; i < 16; i++) {
        setf(0x180 + (uint32_t)i, 0, 0, 0, 0);
    }
    mem(0x182)->f[0] = -1;
    mem(0x183)->f[0] = 1;
    setf(0x187, point_on ? 0.125f : 1, 1, 1, 1);
    setf(0x188, 100, 50, 25, 0);
    setf(0x18C, 40, 80, 120, 10);
}

static void run_vu1_lit(int point_on, float spec, float amb_w) {
    Hdr h = scene_header(1, 0);
    int i;
    h.uvoff[3] = spec;
    h.amb[3] = amb_w;
    put_header(&h, 1);
    scene_positions(0);
    put_tri(0, 0, 1, 2, 1);
    grx_vu1_run(0x130 / 8); /* phase 1: positions */
    for (i = 0; i < 3; i++) {
        seti(0x80 + (uint32_t)i, 0, 0, -4096, 0);
    }
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 2: normals */
    CHECK(mem(0x80)->f[2] == -1.0f, "lit: normal z = %g", mem(0x80)->f[2]);
    seti(0x100, 1, 2, 3, 4);
    seti(0x101, 0, 0, 0, 0);
    seti(0x102, 0, 0, 0, 0);
    seti(0x103, 7, 7, 7, 7); /* slot count: the directional-only loop lights it too */
    lit_lights(point_on);
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 3: light */
    scene_uvs();
    put_tri(0, 0, 1, 2, 1); /* over the light block */
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 4: project */
    CHECK(gif_calls == 0, "lit: kicked early");
    grx_vu1_run(GRX_VU1_CONTINUE); /* phase 5: assemble */
    check_tag("lit", 1);
}

static void test_vu1_lit(void) {
    static const int32_t point[4] = {131, 112, 118, 9}, dir[4] = {111, 72, 58, 4};
    int i, on;
    for (on = 1; on >= 0; on--) {
        const int32_t *e = on ? point : dir;
        reset_vu();
        run_vu1_lit(on, 0, 0);
        for (i = 0; i < 4; i++) {
            CHECK((int32_t)gif[2][i] == e[i], "lit (point %s): v0 RGBA[%d] = %d, expected %d", on ? "on" : "off", i,
                  (int32_t)gif[2][i], e[i]);
        }
        /* slot 3 = count: lit by the directional-only loop (7 + 100*a), not otherwise */
        CHECK(on ? mem(0x103)->i[0] == 7 : mem(0x103)->f[0] == 7.0f + 100.0f * 0.0f,
              "lit (point %s): slot 3 = 0x%X", on ? "on" : "off", mem(0x103)->u[0]);
        CHECK((int32_t)gif[3][0] == exp_xyzf[0][0] && (int32_t)gif[3][2] == exp_xyzf[0][2], "lit: XYZ changed");
    }
}

/* Specular: L0 = (0, 0, 1), I0 = 1, factor -100. Vertex v0: P (0, 0, 4),
   N (0, 0, -1): P.N = -4, N.(-L) = 1, P.(-L) = -4, r = -4 - 2 (-4)(1) = 4,
   (r^2 / |P|^2)^2 = 1, alpha = -100 x -1 x 1 = 100; the ambient alpha (50)
   is zeroed. The directional term is max(0, -1) = 0. v1 at (1, 0, 4):
   P.N = -4, P.(-L) = -4, r = 4, r^2 / |P|^2 = 16/17, squared 0.8858,
   alpha 88.58 -> 88. */
static void test_vu1_specular(void) {
    reset_vu();
    {
        Hdr h = scene_header(1, 0);
        h.uvoff[3] = -100;
        h.amb[3] = 50;
        put_header(&h, 1);
    }
    scene_positions(0);
    grx_vu1_run(0x130 / 8);
    seti(0x80, 0, 0, -4096, 0);
    seti(0x81, 0, 0, -4096, 0);
    seti(0x82, 0, 0, -4096, 0);
    grx_vu1_run(GRX_VU1_CONTINUE);
    seti(0x100, 1, 2, 3, 4);
    seti(0x101, 0, 0, 0, 0);
    seti(0x102, 0, 0, 0, 0);
    lit_lights(0);
    mem(0x182)->f[0] = 1;
    grx_vu1_run(GRX_VU1_CONTINUE);
    scene_uvs();
    put_tri(0, 0, 1, 2, 1);
    grx_vu1_run(GRX_VU1_CONTINUE);
    grx_vu1_run(GRX_VU1_CONTINUE);
    check_tag("specular", 1);
    CHECK((int32_t)gif[2][3] == 100, "specular: v0 alpha = %d, expected 100", (int32_t)gif[2][3]);
    CHECK((int32_t)gif[5][3] == 88, "specular: v1 alpha = %d, expected 88", (int32_t)gif[5][3]);
    CHECK((int32_t)gif[2][0] == 11 && (int32_t)gif[2][1] == 22 && (int32_t)gif[2][2] == 33,
          "specular: v0 RGB = %d %d %d, expected 11 22 33", (int32_t)gif[2][0], (int32_t)gif[2][1],
          (int32_t)gif[2][2]);
}

/* ---- VU0 paths: view-space positions, a used-vertex list in the w words
   of slots [first, count), raw indices ---- */

static void vu0_mesh(uint32_t ntri, uint32_t mode, int lit) {
    Hdr h = scene_header(ntri, mode);
    /* 5 slots; the list in slots 2..4 names vertices 0, 1, 2 (entry j <= j,
       as GphVertCompact builds it); vertex 3, 4 unused */
    h.first = 2, h.count = 5;
    put_header(&h, 0);
    scene_positions(1);
    setf(3, 9, 9, 9, 0);
    setf(4, 9, 9, 9, 0);
    mem(0)->i[3] = 0, mem(1)->i[3] = 0, mem(2)->i[3] = 0, mem(3)->i[3] = 1, mem(4)->i[3] = 2;
    (void)lit;
}

static void test_vu0_unlit(void) {
    int i;
    reset_vu();
    vu0_mesh(2, 0, 0);
    scene_colours();
    seti(0x103, 77, 0, 0, 0);
    grx_vu1_run(0x10 / 8); /* MSCAL 2 */
    for (i = 5; i < 12; i++) {
        CHECK(mem((uint32_t)i)->u[3] == 0x7F, "vu0 unlit: pad slot %d w = 0x%X", i, mem((uint32_t)i)->u[3]);
    }
    CHECK(mem(0x103)->f[0] == 77.0f, "vu0 unlit: colour 3 not converted");
    scene_uvs();
    put_tri(0, 0, 1, 2, 0);
    put_tri(1, 0, 2, 1, 0);
    grx_vu1_run(GRX_VU1_CONTINUE);
    CHECK(mem(3)->f[0] == 9.0f && mem(0x103)->f[0] == 77.0f, "vu0 unlit: unlisted vertex 3 projected");
    grx_vu1_run(GRX_VU1_CONTINUE);
    check_tag("vu0 unlit", 2);
    check_vertex("vu0 unlit", 0, 0, 0);
    check_vertex("vu0 unlit", 0, 1, 1);
    check_vertex("vu0 unlit", 0, 2, 2);
    check_vertex("vu0 unlit", 1, 1, 2);
    /* no culling on the VU0 paths */
    CHECK(xyzf_w(0, 2) == 39 && xyzf_w(1, 2) == 35, "vu0 unlit: culled");
}

/* The VU0 path with the sort flag: the keys are computed, but the key
   pass's branch-in-delay-slot returns without sorting (as PCSX2 runs it) */
static void test_vu0_sort_flag(void) {
    reset_vu();
    vu0_mesh(2, 0x40, 0);
    scene_colours();
    grx_vu1_run(0x10 / 8);
    scene_uvs();
    put_tri(0, 0, 2, 2, 0); /* key 8 + 16 + 16 = 40 */
    put_tri(1, 0, 1, 0, 0); /* key 24 */
    grx_vu1_run(GRX_VU1_CONTINUE);
    CHECK(mem(0x17F)->f[3] == 40.0f && mem(0x180)->f[3] == 24.0f, "vu0 sort flag: keys %g %g, expected 40 24",
          mem(0x17F)->f[3], mem(0x180)->f[3]);
    grx_vu1_run(GRX_VU1_CONTINUE);
    check_tag("vu0 sort flag", 2);
    CHECK(gif[3 + 3][0] == (uint32_t)exp_xyzf[2][0] && gif[3 + 9 + 3][0] == (uint32_t)exp_xyzf[1][0],
          "vu0 sort flag: triangles reordered");
}

static void test_vu0_lit(void) {
    static const int32_t e[4] = {131, 112, 118, 9};
    int i;
    reset_vu();
    {
        Hdr h = scene_header(1, 0);
        h.first = 2, h.count = 5;
        put_header(&h, 0);
    }
    scene_positions(1);
    mem(0)->i[3] = 0, mem(1)->i[3] = 0, mem(2)->i[3] = 0, mem(3)->i[3] = 1, mem(4)->i[3] = 2;
    for (i = 0; i < 5; i++) {
        seti(0x80 + (uint32_t)i, 0, 0, -4096, 0);
    }
    grx_vu1_run(0x30 / 8); /* MSCAL 6: normals of the listed vertices */
    CHECK(mem(0x80)->f[2] == -1.0f && mem(0x82)->f[2] == -1.0f, "vu0 lit: listed normal not transformed");
    CHECK(mem(0x83)->i[2] == -4096, "vu0 lit: unlisted normal transformed");
    seti(0x100, 1, 2, 3, 4);
    seti(0x101, 0, 0, 0, 0);
    seti(0x102, 0, 0, 0, 0);
    seti(0x103, 5, 5, 5, 5);
    lit_lights(1);
    grx_vu1_run(GRX_VU1_CONTINUE); /* light */
    CHECK(mem(0x103)->i[0] == 5, "vu0 lit: unlisted vertex lit");
    scene_uvs();
    put_tri(0, 0, 1, 2, 0);
    grx_vu1_run(GRX_VU1_CONTINUE); /* project */
    grx_vu1_run(GRX_VU1_CONTINUE); /* assemble */
    check_tag("vu0 lit", 1);
    for (i = 0; i < 4; i++) {
        CHECK((int32_t)gif[2][i] == e[i], "vu0 lit: v0 RGBA[%d] = %d, expected %d", i, (int32_t)gif[2][i], e[i]);
    }
}

/* MSCNT with nothing to continue, and after a kick, is logged, not run */
static void test_misc(void) {
    reset_vu();
    logs = 0;
    grx_vu1_run(0x20 / 8); /* a dead entry: logged */
    CHECK(logs == 1 && gif_calls == 0, "dead entry: %d logs", logs);
}


/* ---- VU0 kernels ---- */

static void vf_set(int r, float x, float y, float z, float w) {
    grx_vf[r].f[0] = x, grx_vf[r].f[1] = y, grx_vf[r].f[2] = z, grx_vf[r].f[3] = w;
}

static int vf_is(int r, float x, float y, float z, float w) {
    return grx_vf[r].f[0] == x && grx_vf[r].f[1] == y && grx_vf[r].f[2] == z && grx_vf[r].f[3] == w;
}

static void show_vf(const char *what, int r) {
    printf("  %s: vf%02d = (%g, %g, %g, %g)\n", what, r, grx_vf[r].f[0], grx_vf[r].f[1], grx_vf[r].f[2],
           grx_vf[r].f[3]);
}

/* 0x000: vf08 = vf04 x + vf05 y + vf06 z + vf07 w (columns) */
static void test_vu0_mtxmul(void) {
    vf_set(4, 1, 2, 0, 0);
    vf_set(5, 0, 1, 0, 0);
    vf_set(6, 0, 0, 3, 0);
    vf_set(7, 10, 20, 30, 1);
    vf_set(8, 1, 1, 1, 1);
    vf_set(9, 2, 0, 0, 0);
    vf_set(10, 0, 0, 0, 1);
    vf_set(11, 0, 1, 1, 2);
    grx_vu0_callms(0x000);
    CHECK(vf_is(8, 11, 23, 33, 1), "vu0 0x000: vf08");
    CHECK(vf_is(9, 2, 4, 0, 0), "vu0 0x000: vf09");
    CHECK(vf_is(10, 10, 20, 30, 1), "vu0 0x000: vf10");
    CHECK(vf_is(11, 20, 41, 63, 2), "vu0 0x000: vf11");
}

/* VuProjectVerts: 0x100 starts vertex 0 (vf08), 0x140 finishes it into
   vf12 with the Q its DIV left, transforms vf09 and vf10 and finishes vf09
   into vf13 with the Q of a DIV 7 pairs earlier; 0x1C0 finishes vf10 and
   vf11. Matrix: translate z by 5; scale (2, 3, 7), offset (100, 200, 300),
   distance 10:
     v (1, 2, 0)  -> (1, 2, 5),  q 2: (100 + 2 1 2, 200 + 3 2 2, 300 + 7 2) = (104, 212, 314)
     v (3, -1, 5) -> (3, -1, 10), q 1: (106, 197, 307)
     v (0, 0, -3) -> (0, 0, 2),  q 5: (100, 200, 335)
     v (1, 1, 15) -> (1, 1, 20), q 0.5: (101, 201.5, 303.5) */
static void test_vu0_project(void) {
    vf_set(4, 1, 0, 0, 0);
    vf_set(5, 0, 1, 0, 0);
    vf_set(6, 0, 0, 1, 0);
    vf_set(7, 0, 0, 5, 1);
    vf_set(16, 2, 3, 7, 0);
    vf_set(17, 100, 200, 300, 0);
    vf_set(18, 0, 0, 0, 10);
    vf_set(8, 1, 2, 0, 1);
    vf_set(9, 3, -1, 5, 1);
    vf_set(10, 0, 0, -3, 1);
    vf_set(11, 1, 1, 15, 1);
    grx_vu0_callms(0x100);
    grx_vu0_callms(0x140);
    CHECK(vf_is(12, 104, 212, 314, 1), "vu0 project: vertex 0");
    CHECK(vf_is(13, 106, 197, 307, 1), "vu0 project: vertex 1 (Q 7 pairs after its DIV)");
    if (!vf_is(13, 106, 197, 307, 1)) {
        show_vf("vertex 1", 13);
    }
    vf_set(8, 0, 0, 5, 1); /* the next vertex, which 0x1C0 starts: (0, 0, 10), q 1 */
    grx_vu0_callms(0x1C0);
    CHECK(vf_is(14, 100, 200, 335, 1), "vu0 project: vertex 2");
    CHECK(vf_is(15, 101, 201.5f, 303.5f, 1), "vu0 project: vertex 3");
    if (!vf_is(15, 101, 201.5f, 303.5f, 1)) {
        show_vf("vertex 3", 15);
    }
    CHECK(grx_vi[22] == 0x3F800000u, "vu0 project: Q = 0x%08X, expected 10 / 10 = 1", grx_vi[22]);
}

/* 0xAA8/0xB08: vi02/vi03 = 0x80 when (b - a) x (c - a) . a < 0. FMAND in
   the last pair reads the flags of the MADDAZ 4 pairs before it. */
static void test_vu0_facing(void) {
    int flip;
    for (flip = 0; flip < 2; flip++) {
        vf_set(1, 0, 0, 1, 1);
        vf_set(flip ? 3 : 2, 1, 0, 1, 1);
        vf_set(flip ? 2 : 3, 0, 1, 1, 1);
        vf_set(4, 0, 0, 1, 1);
        vf_set(flip ? 5 : 6, 1, 0, 1, 1);
        vf_set(flip ? 6 : 5, 0, 1, 1, 1);
        grx_vu0_callms(0xAA8);
        grx_vu0_callms(0xB08);
        CHECK(grx_vi[2] == (flip ? 0x80u : 0u), "vu0 0xAA8 (flip %d): vi02 = 0x%X", flip, grx_vi[2]);
        CHECK(grx_vi[3] == (flip ? 0u : 0x80u), "vu0 0xB08 (flip %d): vi03 = 0x%X", flip, grx_vi[3]);
    }
}

/* 0xB68: two triangles at once; vi02 gets the first's flags (the MADDAZ 4
   pairs before the first FMAND), vi03 the second's. It also loads the next
   pair's vertices from VU0 memory, at index & 0x7F, plus 4. */
static void test_vu0_tricull(void) {
    vf_set(1, 0, 0, 1, 1);
    vf_set(2, 1, 0, 1, 1);
    vf_set(3, 0, 1, 1, 1); /* facing +: 0 */
    vf_set(4, 0, 0, 1, 1);
    vf_set(5, 0, 1, 1, 1);
    vf_set(6, 1, 0, 1, 1); /* facing -: 0x80 */
    grx_vf[27].i[0] = 0x81, grx_vf[27].i[1] = 2, grx_vf[27].i[2] = 3;
    grx_vf[28].i[0] = 4, grx_vf[28].i[1] = 5, grx_vf[28].i[2] = 6;
    grx_vu0_callms(0xB68);
    CHECK(grx_vi[2] == 0, "vu0 0xB68: vi02 = 0x%X, expected 0", grx_vi[2]);
    CHECK(grx_vi[3] == 0x80, "vu0 0xB68: vi03 = 0x%X, expected 0x80", grx_vi[3]);
}

/* 0x748: a billboard centre (0, 0, 2) with half-sizes (3, 1.5), identity
   matrices, q = 2 / max(2, near 0.5) = 1, guard band 2.5. Corners:
     vf16 (-3, -1.5, 2): vs 1: -x -y +z = 0x1A, vs 2.5: -x = 0x02 -> vi08 0x682
     vf17 (-3,  1.5, 2): 0x16, 0x02 -> vi09 0x582
     vf18 ( 3,  1.5, 2): 0x15, 0x01 -> vi10 0x541
     vf19 ( 3, -1.5, 2): 0x19, 0x01 -> vi11 0x641
   Each FCGET sees the clip flag as of 4 pairs earlier: one corner's two
   judgements. */
static void test_vu0_billboard(void) {
    grx_vi[18] = 0;
    vf_set(4, 1, 0, 0, 0);
    vf_set(5, 0, 1, 0, 0);
    vf_set(6, 0, 0, 1, 0);
    vf_set(7, 0, 0, 0, 1);
    vf_set(10, 0, 0, 2, 1);
    vf_set(8, 1, 0, 0, 0);
    vf_set(9, 0, 1, 0, 0);
    vf_set(11, 3, 1.5f, 0, 0);
    vf_set(1, 1, 1, 1, 2);
    vf_set(2, 0, 0, 0, 2.5f);
    vf_set(3, 0, 0, 0, 0.5f);
    grx_vi[3] = 0x10;
    grx_vu0_callms(0x748);
    CHECK(vf_is(16, -3, -1.5f, 2, grx_vf[16].f[3]) && vf_is(18, 3, 1.5f, 2, grx_vf[18].f[3]),
          "vu0 billboard: corners");
    if (!vf_is(16, -3, -1.5f, 2, grx_vf[16].f[3])) {
        show_vf("corner 0", 16);
    }
    CHECK(grx_vi[8] == 0x682 && grx_vi[9] == 0x582 && grx_vi[10] == 0x541 && grx_vi[11] == 0x641,
          "vu0 billboard: clip codes 0x%X 0x%X 0x%X 0x%X, expected 0x682 0x582 0x541 0x641", grx_vi[8], grx_vi[9],
          grx_vi[10], grx_vi[11]);
    CHECK(grx_vi[3] == 0x14, "vu0 billboard: vi03 = 0x%X after 4 SQI", grx_vi[3]);
    /* the next call stores these corners (sqi.xy) and turns the codes into
       the w words: (~code & 0x3F0) | (code & 0xF) */
    grx_vu0_callms(0x890);
    CHECK(grx_vf[24].i[3] == ((~0x682 & 0x3F0) | (0x682 & 0xF)), "vu0 billboard: vf24.w = 0x%X", grx_vf[24].i[3]);
    CHECK(grx_vf[27].i[3] == ((~0x641 & 0x3F0) | (0x641 & 0xF)), "vu0 billboard: vf27.w = 0x%X", grx_vf[27].i[3]);
    CHECK(grx_vi[3] == 0x18, "vu0 billboard: vi03 = 0x%X", grx_vi[3]);
}

static void test_vu0_unknown(void) {
    logs = 0;
    grx_vu0_callms(0x123);
    CHECK(logs == 1, "vu0: unknown kernel not logged");
}

int main(void) {
    int n;
    test_vu1_unlit_nocull();
    test_vu1_cull();
    test_vu1_sort_one();
    for (n = 2; n <= 64; n += (n < 8 ? 1 : 7)) {
        test_vu1_sort(n);
    }
    test_vu1_sort(64);
    test_vu1_lit();
    test_vu1_specular();
    test_vu0_unlit();
    test_vu0_sort_flag();
    test_vu0_lit();
    test_misc();
    test_vu0_mtxmul();
    test_vu0_project();
    test_vu0_facing();
    test_vu0_tricull();
    test_vu0_billboard();
    test_vu0_unknown();
    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
