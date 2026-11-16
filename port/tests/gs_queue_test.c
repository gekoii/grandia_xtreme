/* Test of the GS's draw queue (port/src/gs/gs_queue.c): one long stream
   of GIF data that runs into every case where the queue has to order work
   (textures swapped in the same memory while draws still read the old
   one, CLUTs uploaded into pages queued draws read and loaded at once,
   copies of the frame onto itself, uploads into the frame being drawn,
   draws into the same memory through different buffers, local-to-local
   copies, read-back), with big draws in front so the workers are still
   busy when the next piece comes. It prints a hash of local memory, of the
   read-back data and of the displayed picture after each step.

   The output must be the same whatever the number of worker threads, and
   the same as the software GS's before the queue (any build of
   port/src/gs/ that has gs_flush, even a no-op one):
     GRX_GS_THREADS=0 gs_queue_test.exe > a.txt
     gs_queue_test.exe > b.txt          (the default: a worker per CPU)
     GRX_GS_THREADS=3 GRX_GS_BAND=2 gs_queue_test.exe > c.txt
   then compare the files.

   Build as port/tests/gs_test.c. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_gs.h"
#include "gs_internal.h"

static uint8_t priv[0x2000] __attribute__((aligned(16)));
static uint64_t pkt[1 << 22];
static uint32_t np; /* in 64-bit words */
static uint32_t rng = 12345;

static uint32_t rnd(void) {
    rng = rng * 1103515245u + 12345u;
    return rng >> 8;
}

static uint64_t fnv(const void *p, size_t n, uint64_t h) {
    const uint8_t *b = p;
    size_t i;
    for (i = 0; i < n; i++)
        h = (h ^ b[i]) * 0x100000001B3ull;
    return h;
}

static void q(uint64_t lo, uint64_t hi) {
    pkt[np++] = lo;
    pkt[np++] = hi;
}
static void tag(uint32_t nloop, int eop, int pre, uint32_t prim, int flg, int nreg, uint64_t regs) {
    q((uint64_t)nloop | ((uint64_t)eop << 15) | ((uint64_t)pre << 46) | ((uint64_t)prim << 47) | ((uint64_t)flg << 58) |
          ((uint64_t)(nreg & 15) << 60),
      regs);
}
/* A+D register writes go into the current packet; send() hands it over
   without waiting for the GS */
static void set(uint32_t reg, uint64_t data) {
    tag(1, 0, 0, 0, 0, 1, 0xE);
    q(data, reg);
}
static void send(void) {
    if (np)
        grx_gs_gif(pkt, np / 2, GRX_GIF_PATH3);
    np = 0;
}

#define PRIM(type, iip, tme, fge, abe, fst) \
    ((uint64_t)(type) | ((iip) << 3) | ((tme) << 4) | ((fge) << 5) | ((abe) << 6) | ((fst) << 8))

static uint64_t frame(uint32_t fbp, uint32_t fbw, uint32_t psm) {
    return fbp | ((uint64_t)fbw << 16) | ((uint64_t)psm << 24);
}
static uint64_t tex0(uint32_t tbp, uint32_t tbw, uint32_t psm, uint32_t tw, uint32_t th, uint32_t tfx, uint32_t cbp,
                     uint32_t cpsm, uint32_t cld) {
    return tbp | ((uint64_t)tbw << 14) | ((uint64_t)psm << 20) | ((uint64_t)tw << 26) | ((uint64_t)th << 30) |
           (1ull << 34) | ((uint64_t)tfx << 35) | ((uint64_t)cbp << 37) | ((uint64_t)cpsm << 51) |
           ((uint64_t)cld << 61);
}
static uint64_t xyz(int x, int y) {
    return (uint64_t)(x * 16) | ((uint64_t)(y * 16) << 16);
}
static uint64_t rgba(uint32_t r, uint32_t g, uint32_t b, uint32_t a) {
    return r | (g << 8) | (b << 16) | ((uint64_t)a << 24) | ((uint64_t)0x3F800000 << 32);
}

/* An image upload of random data, w x h pixels of psm at (x, y) */
static void upload(uint32_t bp, uint32_t bw, uint32_t psm, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    uint32_t bits = psm == GS_PSMCT32 ? 32 : psm == GS_PSMCT16 ? 16 : psm == GS_PSMT8 ? 8 : 4;
    uint32_t bytes = w * h * bits / 8, qw = (bytes + 15) / 16, i;
    set(GS_BITBLTBUF, ((uint64_t)bp << 32) | ((uint64_t)bw << 48) | ((uint64_t)psm << 56));
    set(GS_TRXPOS, ((uint64_t)x << 32) | ((uint64_t)y << 48));
    set(GS_TRXREG, w | ((uint64_t)h << 32));
    set(GS_TRXDIR, 0);
    tag(qw, 0, 0, 0, 2, 0, 0);
    for (i = 0; i < qw * 2; i++)
        pkt[np + i] = ((uint64_t)rnd() << 40) ^ ((uint64_t)rnd() << 20) ^ rnd();
    np += qw * 2;
}

/* A sprite, UV or STQ */
static void sprite(uint64_t prim, int x0, int y0, int x1, int y1, uint32_t u0, uint32_t v0, uint32_t u1, uint32_t v1,
                   uint64_t colour) {
    set(GS_PRIM, prim);
    set(GS_RGBAQ, colour);
    set(GS_UV, (uint64_t)(u0 * 16) | ((uint64_t)(v0 * 16) << 16));
    set(GS_XYZ2, xyz(x0, y0));
    set(GS_UV, (uint64_t)(u1 * 16) | ((uint64_t)(v1 * 16) << 16));
    set(GS_XYZ2, xyz(x1, y1));
}
static void sprite_st(uint64_t prim, int x0, int y0, int x1, int y1, float s0, float t0, float s1, float t1,
                      uint64_t colour) {
    uint32_t a, b, c, d;
    memcpy(&a, &s0, 4);
    memcpy(&b, &t0, 4);
    memcpy(&c, &s1, 4);
    memcpy(&d, &t1, 4);
    set(GS_PRIM, prim);
    set(GS_RGBAQ, colour);
    set(GS_ST, a | ((uint64_t)b << 32));
    set(GS_XYZ2, xyz(x0, y0));
    set(GS_ST, c | ((uint64_t)d << 32));
    set(GS_XYZ2, xyz(x1, y1));
}
static void triangles(int n, uint64_t prim) {
    int i, k;
    set(GS_PRIM, prim);
    for (i = 0; i < n; i++)
        for (k = 0; k < 3; k++) {
            set(GS_RGBAQ, rgba(rnd() & 255, rnd() & 255, rnd() & 255, rnd() & 127));
            set(GS_UV, (uint64_t)(rnd() % 4096) | ((uint64_t)(rnd() % 4096) << 16));
            set(GS_XYZ2, (uint64_t)(rnd() % (640 * 16)) | ((uint64_t)(rnd() % (448 * 16)) << 16) |
                             ((uint64_t)(rnd() & 0xFFFFFF) << 32));
        }
}

static void report(const char *what) {
    static uint8_t back[640 * 64 * 4];
    const uint32_t *px;
    int w, h, pitch, y, on;
    uint64_t hv, hd = 0, hr;

    send();
    /* read back a part of the frame through the GS */
    set(GS_BITBLTBUF, 0 | (10ull << 16) | (0ull << 24));
    set(GS_TRXPOS, 0 | (100ull << 16));
    set(GS_TRXREG, 640 | (64ull << 32));
    set(GS_TRXDIR, 1);
    send();
    grx_gs_read_image(back, sizeof(back) / 16);
    hr = fnv(back, sizeof(back), 0xCBF29CE484222325ull);
    on = grx_gs_display(&px, &w, &h, &pitch);
    for (y = 0; on && y < h; y++)
        hd = fnv(px + (size_t)y * pitch, (size_t)w * 4, hd ? hd : 0xCBF29CE484222325ull);
    gs_flush();
    hv = fnv(gs_vram, GS_VRAM_SIZE, 0xCBF29CE484222325ull);
    printf("%-36s vram %016llx  read %016llx  display %016llx\n", what, (unsigned long long)hv,
           (unsigned long long)hr, (unsigned long long)hd);
}

int main(void) {
    int i, j;

    grx_gs_set_priv(priv);
    grx_gs_init();
    /* display circuit 2: the frame at page 0, 640x448 */
    memcpy(priv + GS_PRIV_PMODE, &(uint64_t){0x66}, 8);
    memcpy(priv + GS_PRIV_DISPFB2, &(uint64_t){0 | (10u << 9)}, 8);
    memcpy(priv + GS_PRIV_DISPLAY2, &(uint64_t){0x1BF9FF0183227Cull}, 8);

    set(GS_PRMODECONT, 1);
    set(GS_FRAME_1, frame(0, 10, GS_PSMCT32));
    set(GS_ZBUF_1, 0x100 | (0ull << 24));
    set(GS_XYOFFSET_1, 0);
    set(GS_SCISSOR_1, 0 | (639ull << 16) | (0ull << 32) | (447ull << 48));
    set(GS_TEST_1, (1u << 16) | (2u << 17)); /* ZTE, GEQUAL */
    set(GS_TEXA, 0x80ull | (0x80ull << 32));
    set(GS_COLCLAMP, 1);
    set(GS_ALPHA_1, 0 | (1 << 2) | (0 << 4) | (1 << 6)); /* (Cs - Cd) * As + Cd */
    set(GS_TEX1_1, (1u << 5) | (1u << 6));             /* bilinear */
    set(GS_CLAMP_1, 0);
    /* background and Z */
    upload(0, 10, GS_PSMCT32, 0, 0, 640, 448);
    upload(0x100 * 32, 10, GS_PSMCT32, 0, 0, 640, 448);
    report("frame and Z uploaded");

    /* 1. A texture swapped in the same memory, again and again, with its
       CLUT in the same page: uploads and CLUT loads while draws still read
       the page */
    for (i = 0; i < 12; i++) {
        upload(0x2000, 4, GS_PSMT8, 0, 0, 128, 128);  /* T8 texture, pages 0x100-0x101 */
        upload(0x2000 + 24, 1, GS_PSMCT32, 0, 0, 16, 16); /* its CLUT, in page 0x100 */
        set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMT8, 7, 7, 0, 0x2000 + 24, 0, 1));
        triangles(60, PRIM(3, 1, 1, 0, 1, 1));
        sprite(PRIM(6, 0, 1, 0, 1, 1), 0, 0, 640, 448, 0, 0, 128, 128, rgba(128, 128, 128, 64));
        /* a CLUT loaded with CLD 4 / 5 too */
        upload(0x2000 + 28, 1, GS_PSMCT32, 0, 0, 16, 16);
        set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMT8, 7, 7, 0, 0x2000 + 28, 0, 4 + (i & 1)));
        sprite(PRIM(6, 0, 1, 0, 1, 1), 64, 32, 576, 416, 3, 5, 120, 110, rgba(100, 128, 90, 100));
    }
    report("textures and CLUTs swapped");

    /* 2. T4 textures and 16-bit CLUTs, CSM2 */
    for (i = 0; i < 6; i++) {
        upload(0x2800, 2, GS_PSMT4, 0, 0, 128, 128);
        upload(0x2800 + 16, 4, GS_PSMCT16, 32, 3, 64, 1); /* CLUT line, CSM2 */
        set(GS_TEXCLUT, 4 | (2u << 6) | (3u << 12));
        set(GS_TEX0_1, tex0(0x2800, 2, GS_PSMT4, 7, 7, 0, 0x2800 + 16, GS_PSMCT16, 1) | (1ull << 55) |
                           ((uint64_t)(i & 3) << 56));
        triangles(80, PRIM(4, 1, 1, 0, 1, 1));
    }
    report("T4 and CSM2");

    /* 3. The frame copied onto itself, shifted (reads rows another worker
       draws), and onto the next buffer along, STQ and UV */
    set(GS_TEST_1, 0);
    for (i = 0; i < 4; i++) {
        set(GS_TEX0_1, tex0(0, 10, GS_PSMCT32, 10, 9, 1, 0, 0, 0));
        sprite_st(PRIM(6, 0, 1, 0, 0, 0), 0, 0, 640, 448, 0.0f, 0.0f, 0.625f, 0.876f, rgba(128, 128, 128, 128));
        sprite(PRIM(6, 0, 1, 0, 1, 1), 0, 0, 640, 440, 0, 8 + i, 640, 448 + i, rgba(128, 128, 128, 100));
        sprite(PRIM(6, 0, 1, 0, 1, 1), 0, 8, 640, 448, 0, 0, 640, 440, rgba(128, 128, 128, 100));
    }
    report("frame copied onto itself");

    /* 4. Draws into the same memory as another buffer, other widths and
       formats, and a triangle reading the frame it draws into */
    for (i = 0; i < 4; i++) {
        set(GS_FRAME_1, frame(0, 8, GS_PSMCT32));
        triangles(40, PRIM(3, 1, 0, 0, 0, 0));
        set(GS_FRAME_1, frame(0, 10, GS_PSMCT16));
        triangles(40, PRIM(3, 1, 0, 0, 1, 0));
        set(GS_FRAME_1, frame(0, 10, GS_PSMCT32));
        set(GS_TEX0_1, tex0(0, 10, GS_PSMCT32, 10, 9, 0, 0, 0, 0));
        triangles(20, PRIM(3, 1, 1, 0, 1, 1));
    }
    report("buffers over each other");

    /* 5. Uploads into the frame while draws are queued over it, a
       local-to-local copy, a Z test against uploaded Z */
    set(GS_TEST_1, (1u << 16) | (2u << 17));
    for (i = 0; i < 6; i++) {
        triangles(50, PRIM(3, 1, 0, 0, 0, 0));
        upload(0, 10, GS_PSMCT32, 64 * i, 50 * i, 200, 60);
        upload(0x100 * 32, 10, GS_PSMCT32, 0, 30 * i, 640, 40);
        set(GS_BITBLTBUF, 0 | (10ull << 16) | ((uint64_t)(0x3000) << 32) | (4ull << 48));
        set(GS_TRXPOS, (uint64_t)(10 * i) | (20ull << 16));
        set(GS_TRXREG, 200 | (100ull << 32));
        set(GS_TRXDIR, 2);
        set(GS_TEX0_1, tex0(0x3000, 4, GS_PSMCT32, 8, 7, 0, 0, 0, 0));
        sprite(PRIM(6, 0, 1, 0, 1, 1), 300, 200, 500, 300, 0, 0, 200, 100, rgba(128, 128, 128, 80));
    }
    report("uploads into the frame, copies");

    /* 6. Many small pieces, as VU1 sends them */
    for (j = 0; j < 30; j++) {
        upload(0x2000, 4, GS_PSMT8, 0, 0, 128, 64);
        upload(0x2000 + 24, 1, GS_PSMCT32, 0, 0, 16, 16);
        set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMT8, 7, 6, 0, 0x2000 + 24, 0, 1));
        for (i = 0; i < 10; i++) {
            triangles(3, PRIM(3, 1, 1, 0, 1, 1));
            send();
        }
    }
    report("small pieces");
    return 0;
}
