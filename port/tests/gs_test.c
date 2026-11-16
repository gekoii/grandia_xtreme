/* Test of the software GS (port/src/gs/): draws known scenes through the
   GIF and checks pixels. Standalone: the privileged registers are a plain
   buffer (grx_gs_set_priv).

   Build and run (from the repository root; any output directory; the
   GS's worker threads use SDL3, so SDL3.dll goes next to the exe):
     i686-w64-mingw32-clang -std=gnu11 -Wall -O1 -msse2 -mfpmath=sse \
         -Iport/include -Iport/src/gs -Iport/deps/SDL3-3.4.18/i686-w64-mingw32/include \
         port/tests/gs_test.c port/src/gs/gs_*.c \
         -Lport/deps/SDL3-3.4.18/i686-w64-mingw32/lib -lSDL3 -o gs_test.exe
     ./gs_test.exe [snapshot-dir]
   GRX_GS_THREADS=0 draws on the calling thread (no worker threads).

   With a directory, PPM snapshots of some scenes are written there. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grx_gs.h"
#include "gs_internal.h"

static int failures, checks;
static const char *snapdir;

#define CHECK(cond, ...)                                  \
    do {                                                  \
        checks++;                                         \
        if (!(cond)) {                                    \
            failures++;                                   \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);   \
            printf(__VA_ARGS__);                          \
            printf("\n");                                 \
        }                                                 \
    } while (0)

static uint8_t priv[0x2000] __attribute__((aligned(16)));

/* ---- GIF packets ---- */

static uint64_t pkt[1 << 20];
static uint32_t np; /* in 64-bit words */

static void q(uint64_t lo, uint64_t hi) {
    pkt[np++] = lo;
    pkt[np++] = hi;
}
static void tag(uint32_t nloop, int eop, int pre, uint32_t prim, int flg, int nreg, uint64_t regs) {
    q((uint64_t)nloop | ((uint64_t)eop << 15) | ((uint64_t)pre << 46) | ((uint64_t)prim << 47) | ((uint64_t)flg << 58) |
          ((uint64_t)(nreg & 15) << 60),
      regs);
}
static void flush(void) {
    if (np)
        grx_gs_gif(pkt, np / 2, GRX_GIF_PATH3);
    np = 0;
    gs_flush();
}
/* A+D writes, one tag each */
static void set(uint32_t reg, uint64_t data) {
    tag(1, 1, 0, 0, 0, 1, 0xE);
    q(data, reg);
    flush();
}

#define PRIM(type, iip, tme, fge, abe, fst) \
    ((uint64_t)(type) | ((iip) << 3) | ((tme) << 4) | ((fge) << 5) | ((abe) << 6) | ((fst) << 8))

static uint64_t frame(uint32_t fbp, uint32_t fbw, uint32_t psm, uint32_t msk) {
    return fbp | ((uint64_t)fbw << 16) | ((uint64_t)psm << 24) | ((uint64_t)msk << 32);
}
static uint64_t tex0(uint32_t tbp, uint32_t tbw, uint32_t psm, uint32_t tw, uint32_t th, uint32_t tcc, uint32_t tfx,
                     uint32_t cbp, uint32_t cpsm, uint32_t csm, uint32_t csa, uint32_t cld) {
    return tbp | ((uint64_t)tbw << 14) | ((uint64_t)psm << 20) | ((uint64_t)tw << 26) | ((uint64_t)th << 30) |
           ((uint64_t)tcc << 34) | ((uint64_t)tfx << 35) | ((uint64_t)cbp << 37) | ((uint64_t)cpsm << 51) |
           ((uint64_t)csm << 55) | ((uint64_t)csa << 56) | ((uint64_t)cld << 61);
}
static uint64_t scissor(int x0, int x1, int y0, int y1) {
    return (uint64_t)x0 | ((uint64_t)x1 << 16) | ((uint64_t)y0 << 32) | ((uint64_t)y1 << 48);
}
static uint64_t xyz(int x, int y, uint32_t z) { /* pixels, XYOFFSET 0 */
    return (uint64_t)(x * 16) | ((uint64_t)(y * 16) << 16) | ((uint64_t)z << 32);
}
static uint64_t uv(int u, int v) {
    return (uint64_t)(u * 16) | ((uint64_t)(v * 16) << 16);
}
static uint64_t rgba(uint32_t r, uint32_t g, uint32_t b, uint32_t a) {
    return r | (g << 8) | (b << 16) | ((uint64_t)a << 24) | ((uint64_t)0x3F800000 << 32);
}

/* A plain context 1: 640x448 CT32 frame at page 0, Z32 at page 0x100,
   full scissor, no tests, no blending */
static void basic_state(uint32_t fpsm) {
    set(GS_PRMODECONT, 1);
    set(GS_FRAME_1, frame(0, 10, fpsm, 0));
    set(GS_ZBUF_1, 0x100 | (0ull << 24) | (1ull << 32));
    set(GS_XYOFFSET_1, 0);
    set(GS_SCISSOR_1, scissor(0, 639, 0, 447));
    set(GS_TEST_1, 0);
    set(GS_ALPHA_1, 0);
    set(GS_TEXA, 0x80ull | (0x80ull << 32));
    set(GS_TEX1_1, 0);
    set(GS_CLAMP_1, 0);
    set(GS_COLCLAMP, 1);
    set(GS_DTHE, 0);
    set(GS_PABE, 0);
    set(GS_FBA_1, 0);
    set(GS_SCANMSK, 0);
}

static void fill(int x0, int y0, int x1, int y1, uint32_t r, uint32_t g, uint32_t b, uint32_t a) {
    set(GS_PRIM, PRIM(6, 0, 0, 0, 0, 0));
    set(GS_RGBAQ, rgba(r, g, b, a));
    set(GS_XYZ2, xyz(x0, y0, 0));
    set(GS_XYZ2, xyz(x1, y1, 0));
}

static void upload(uint32_t bp, uint32_t bw, uint32_t psm, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                   const void *data, uint32_t bytes) {
    uint32_t qw = (bytes + 15) / 16;
    set(GS_BITBLTBUF, ((uint64_t)bp << 32) | ((uint64_t)bw << 48) | ((uint64_t)psm << 56));
    set(GS_TRXPOS, ((uint64_t)x << 32) | ((uint64_t)y << 48));
    set(GS_TRXREG, w | ((uint64_t)h << 32));
    set(GS_TRXDIR, 0);
    tag(qw, 1, 0, 0, 2, 0, 0);
    memset(&pkt[np], 0, qw * 16);
    memcpy(&pkt[np], data, bytes);
    np += qw * 2;
    flush();
}

static void download(uint32_t bp, uint32_t bw, uint32_t psm, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                     void *data, uint32_t bytes) {
    set(GS_BITBLTBUF, bp | ((uint64_t)bw << 16) | ((uint64_t)psm << 24));
    set(GS_TRXPOS, x | ((uint64_t)y << 16));
    set(GS_TRXREG, w | ((uint64_t)h << 32));
    set(GS_TRXDIR, 1);
    grx_gs_read_image(data, (bytes + 15) / 16);
}

static uint32_t px32(uint32_t fbp, int x, int y) {
    return gs_read_pixel(GS_PSMCT32, fbp * 32, 10, x, y);
}

static void snapshot(const char *name, uint32_t psm, uint32_t fbp, int w, int h) {
    char path[512];
    FILE *f;
    int x, y;
    if (!snapdir)
        return;
    snprintf(path, sizeof(path), "%s/%s.ppm", snapdir, name);
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
            uint32_t c = gs_read_pixel(psm, fbp * 32, 10, x, y);
            unsigned char rgb[3];
            if (psm == GS_PSMCT16 || psm == GS_PSMCT16S) {
                rgb[0] = (c & 31) << 3;
                rgb[1] = ((c >> 5) & 31) << 3;
                rgb[2] = ((c >> 10) & 31) << 3;
            } else {
                rgb[0] = c & 255;
                rgb[1] = (c >> 8) & 255;
                rgb[2] = (c >> 16) & 255;
            }
            fwrite(rgb, 1, 3, f);
        }
    fclose(f);
}

static uint32_t rng = 12345;
static uint32_t rnd(void) {
    rng = rng * 1103515245u + 12345u;
    return (rng >> 8) ^ (rng << 13);
}

/* ---- tests ---- */

/* Swizzle tables against PCSX2's columnTable8/columnTable4 (rows 0, 2, 4, 10) */
static void test_tables(void) {
    static const uint16_t ct8_r2[16] = {33, 37, 49, 53, 1, 5, 17, 21, 35, 39, 51, 55, 3, 7, 19, 23};
    static const uint16_t ct8_r4[16] = {96, 100, 112, 116, 64, 68, 80, 84, 98, 102, 114, 118, 66, 70, 82, 86};
    static const uint16_t ct8_r10[16] = {161, 165, 177, 181, 129, 133, 145, 149,
                                         163, 167, 179, 183, 131, 135, 147, 151};
    static const uint16_t ct4_r2[32] = {65, 73, 97, 105, 1, 9, 33, 41, 67, 75, 99, 107, 3, 11, 35, 43,
                                        69, 77, 101, 109, 5, 13, 37, 45, 71, 79, 103, 111, 7, 15, 39, 47};
    static const uint16_t ct4_r4[8] = {192, 200, 224, 232, 128, 136, 160, 168};
    static const uint16_t ct16_r1[16] = {4, 6, 12, 14, 20, 22, 28, 30, 5, 7, 13, 15, 21, 23, 29, 31};
    static const uint16_t ct32_r3[8] = {18, 19, 22, 23, 26, 27, 30, 31};
    int x;
    for (x = 0; x < 16; x++) {
        CHECK(gs_pg8[2][x] == ct8_r2[x], "ct8 row 2 x %d: %u", x, gs_pg8[2][x]);
        CHECK(gs_pg8[4][x] == ct8_r4[x], "ct8 row 4 x %d: %u", x, gs_pg8[4][x]);
        CHECK(gs_pg8[10][x] == ct8_r10[x], "ct8 row 10 x %d: %u", x, gs_pg8[10][x]);
        CHECK(gs_pg16[0][1][x] == ct16_r1[x], "ct16 row 1 x %d", x);
    }
    for (x = 0; x < 32; x++)
        CHECK(gs_pg4[2][x] == ct4_r2[x], "ct4 row 2 x %d: %u", x, gs_pg4[2][x]);
    for (x = 0; x < 8; x++) {
        CHECK(gs_pg4[4][x] == ct4_r4[x], "ct4 row 4 x %d", x);
        CHECK(gs_pg32[0][3][x] == ct32_r3[x], "ct32 row 3 x %d", x);
    }
    /* Block order: CT32 block at (8,0) is 1, at (0,8) is 2, at (32,0) 16;
       Z32 the same xor 0x18 */
    CHECK(gs_pg32[0][0][8] == 1 * 64 && gs_pg32[0][8][0] == 2 * 64 && gs_pg32[0][0][32] == 16 * 64, "bt32");
    CHECK(gs_pg32[1][0][0] == 24 * 64, "bt32z");
    CHECK(gs_pg16[1][0][32] == 16 * 128 && gs_pg16[1][32][0] == 4 * 128, "bt16s");
}

/* Upload, read back, in every format; then cross-format views */
static void test_roundtrip(void) {
    static const uint32_t psms[] = {GS_PSMCT32, GS_PSMCT24, GS_PSMCT16, GS_PSMCT16S, GS_PSMT8, GS_PSMT4,
                                    GS_PSMT8H, GS_PSMT4HL, GS_PSMT4HH, GS_PSMZ32, GS_PSMZ24, GS_PSMZ16,
                                    GS_PSMZ16S};
    static uint8_t in[256 * 256 * 4], out[256 * 256 * 4];
    unsigned i, k;

    for (k = 0; k < sizeof(psms) / sizeof(psms[0]); k++) {
        uint32_t psm = psms[k];
        uint32_t bits = psm == GS_PSMCT32 || psm == GS_PSMZ32 ? 32 : psm == GS_PSMCT24 || psm == GS_PSMZ24 ? 24
                        : (psm & 0xF) == 2 || (psm & 0xF) == 0xA ? 16 : psm == GS_PSMT8 || psm == GS_PSMT8H ? 8 : 4;
        /* odd position and size, crossing pages; width 200 needs 4 units */
        uint32_t w = 200, h = 150, x = 37, y = 21, bw = 6;
        uint32_t bytes = w * h * bits / 8;
        for (i = 0; i < bytes; i++)
            in[i] = (uint8_t)rnd();
        upload(0x800, bw, psm, x, y, w, h, in, bytes);
        memset(out, 0xEE, bytes);
        download(0x800, bw, psm, x, y, w, h, out, bytes);
        CHECK(memcmp(in, out, bytes) == 0, "round trip psm 0x%02X", psm);
        /* and the pixel accessor agrees with the transfer's order */
        if (bits == 32)
            CHECK(gs_read_pixel(psm, 0x800, bw, x + 5, y + 3) ==
                      (uint32_t)(in[(3 * w + 5) * 4] | in[(3 * w + 5) * 4 + 1] << 8 | in[(3 * w + 5) * 4 + 2] << 16 |
                                 (uint32_t)in[(3 * w + 5) * 4 + 3] << 24),
                  "accessor psm 0x%02X", psm);
    }

    /* Cross-format (from PCSX2's tables): in one block, PSMT8 pixel (0,2)
       is byte 1 of PSMCT32 pixel (4,0); PSMCT16 (8,0) is the high half
       of PSMCT32 (0,0); PSMT4 (0,2) the high nibble of byte 0 of CT32 (4,0)... */
    {
        uint32_t blk[64];
        for (i = 0; i < 64; i++)
            blk[i] = rnd();
        upload(0x1000, 1, GS_PSMCT32, 0, 0, 8, 8, blk, sizeof(blk));
        CHECK(gs_read_pixel(GS_PSMT8, 0x1000, 2, 0, 2) == ((blk[0 * 8 + 4] >> 8) & 255), "T8 (0,2) vs CT32 (4,0)");
        CHECK(gs_read_pixel(GS_PSMT8, 0x1000, 2, 8, 0) == ((blk[0] >> 16) & 255), "T8 (8,0) vs CT32 (0,0)");
        CHECK(gs_read_pixel(GS_PSMCT16, 0x1000, 1, 8, 0) == (blk[0] >> 16), "CT16 (8,0) vs CT32 (0,0)");
        CHECK(gs_read_pixel(GS_PSMCT16, 0x1000, 1, 0, 1) == (blk[1 * 8 + 0] & 0xFFFF), "CT16 (0,1) vs CT32 (0,1)");
        /* T4 row 2 x 0 is nibble 65 = word 8 nibble 1: CT32 (4,0) bits 4-7 */
        CHECK(gs_read_pixel(GS_PSMT4, 0x1000, 2, 0, 2) == ((blk[4] >> 4) & 15), "T4 (0,2) vs CT32 (4,0)");
        CHECK(gs_read_pixel(GS_PSMT8H, 0x1000, 1, 3, 5) == blk[5 * 8 + 3] >> 24, "T8H");
        CHECK(gs_read_pixel(GS_PSMT4HH, 0x1000, 1, 6, 7) == blk[7 * 8 + 6] >> 28, "T4HH");
        /* The block's 256 bytes as T8 are a permutation of the same bytes */
        {
            unsigned cnt[256] = {0}, cnt2[256] = {0};
            int xx, yy;
            for (i = 0; i < 256; i++)
                cnt[((uint8_t *)blk)[i]]++;
            for (yy = 0; yy < 16; yy++)
                for (xx = 0; xx < 16; xx++)
                    cnt2[gs_read_pixel(GS_PSMT8, 0x1000, 2, xx, yy)]++;
            CHECK(memcmp(cnt, cnt2, sizeof(cnt)) == 0, "T8 block is a permutation of CT32 block");
        }
        /* CT24 keeps the T8H byte */
        gs_write_pixel(GS_PSMT8H, 0x1000, 1, 2, 2, 0x5A);
        gs_write_pixel(GS_PSMCT24, 0x1000, 1, 2, 2, 0x123456);
        CHECK(gs_read_pixel(GS_PSMCT32, 0x1000, 1, 2, 2) == 0x5A123456, "CT24 + T8H share a word");
    }

    /* Local to local copy */
    {
        uint32_t img[32 * 16], j;
        for (j = 0; j < 32 * 16; j++)
            img[j] = rnd();
        upload(0x1200, 1, GS_PSMCT32, 0, 0, 32, 16, img, sizeof(img));
        set(GS_BITBLTBUF, 0x1200 | (1ull << 16) | (0ull << 24) | (0x1400ull << 32) | (2ull << 48) | (0ull << 56));
        set(GS_TRXPOS, 0 | (3ull << 32) | (5ull << 48));
        set(GS_TRXREG, 32 | (16ull << 32));
        set(GS_TRXDIR, 2);
        CHECK(gs_read_pixel(GS_PSMCT32, 0x1400, 2, 3 + 7, 5 + 9) == img[9 * 32 + 7], "local to local");
        CHECK(gs_read_pixel(GS_PSMCT32, 0x1400, 2, 3 + 31, 5 + 15) == img[15 * 32 + 31], "local to local corner");
    }
}

/* Textured sprite, 1:1, and a CLUT sprite */
static void test_sprite_texture(void) {
    static uint32_t tex[64 * 64];
    int x, y, bad = 0;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    for (y = 0; y < 64; y++)
        for (x = 0; x < 64; x++)
            tex[y * 64 + x] = (x * 4) | ((y * 4) << 8) | (((x ^ y) & 1 ? 255u : 0u) << 16) | 0x80000000u;
    upload(0x2000, 1, GS_PSMCT32, 0, 0, 64, 64, tex, sizeof(tex));
    set(GS_TEX0_1, tex0(0x2000, 1, GS_PSMCT32, 6, 6, 1, 1, 0, 0, 0, 0, 0));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_RGBAQ, rgba(128, 128, 128, 128));
    set(GS_UV, uv(0, 0));
    set(GS_XYZ2, xyz(100, 50, 0));
    set(GS_UV, uv(64, 64));
    set(GS_XYZ2, xyz(164, 114, 0));
    for (y = 0; y < 64; y++)
        for (x = 0; x < 64; x++)
            if (px32(0, 100 + x, 50 + y) != tex[y * 64 + x])
                bad++;
    CHECK(bad == 0, "textured sprite: %d pixels differ", bad);
    CHECK(px32(0, 99, 50) == 0 && px32(0, 164, 50) == 0 && px32(0, 100, 114) == 0, "sprite edges exclusive");
    CHECK(px32(0, 100, 50) != 0, "sprite top-left inclusive");

    /* The same with STQ (FST = 0), S = T = 0..1, Q = 1 */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 0));
    set(GS_ST, 0);
    set(GS_RGBAQ, rgba(128, 128, 128, 128));
    set(GS_XYZ2, xyz(10, 10, 0));
    set(GS_ST, 0x3F8000003F800000ull);
    set(GS_XYZ2, xyz(74, 74, 0));
    bad = 0;
    for (y = 0; y < 64; y++)
        for (x = 0; x < 64; x++)
            if (px32(0, 10 + x, 10 + y) != tex[y * 64 + x])
                bad++;
    CHECK(bad == 0, "STQ sprite: %d pixels differ", bad);

    /* Modulate with half colour: texel * 64 >> 7 */
    set(GS_TEX0_1, tex0(0x2000, 1, GS_PSMCT32, 6, 6, 1, 0, 0, 0, 0, 0, 0));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_RGBAQ, rgba(64, 64, 64, 128));
    set(GS_UV, uv(0, 0));
    set(GS_XYZ2, xyz(200, 50, 0));
    set(GS_UV, uv(64, 64));
    set(GS_XYZ2, xyz(264, 114, 0));
    CHECK(px32(0, 200 + 40, 50 + 20) == ((40 * 4 / 2) | ((20 * 4 / 2) << 8) | (0u << 16) | 0x80000000u),
          "modulate: %08X", px32(0, 240, 70));

    /* Bilinear with UV at texel centres reproduces the texture */
    set(GS_TEX1_1, (1u << 5) | (1u << 6));
    set(GS_TEX0_1, tex0(0x2000, 1, GS_PSMCT32, 6, 6, 1, 1, 0, 0, 0, 0, 0));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_UV, 8 | (8 << 16));
    set(GS_XYZ2, xyz(300, 50, 0));
    set(GS_UV, (64 * 16 + 8) | ((64 * 16 + 8) << 16));
    set(GS_XYZ2, xyz(364, 114, 0));
    CHECK(px32(0, 300 + 20, 50 + 30) == tex[30 * 64 + 20], "bilinear at centres: %08X vs %08X",
          px32(0, 320, 80), tex[30 * 64 + 20]);
    /* half-way between texel 20 and 21 in u (blue alternates 0/255) */
    set(GS_UV, 16 | (8 << 16));
    set(GS_XYZ2, xyz(400, 50, 0));
    set(GS_UV, (64 * 16 + 16) | ((64 * 16 + 8) << 16));
    set(GS_XYZ2, xyz(464, 114, 0));
    {
        uint32_t c = px32(0, 400 + 20, 50 + 30);
        CHECK((c & 255) == 82 && ((c >> 16) & 255) == 127, "bilinear half-way: %08X", c);
    }
    set(GS_TEX1_1, 0);
    snapshot("sprite_texture", GS_PSMCT32, 0, 640, 200);
}

static void test_clut(void) {
    static uint32_t clut32[256], clut32_4[16 * 2];
    static uint16_t clut16[16];
    static uint8_t idx8[16 * 16], idx4[16 * 16 / 2];
    int i, x, y, bad = 0;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    /* A CSM1 CLUT image: the pixel at (x, y) is (x, y, 0x77, 0x80) */
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++)
            clut32[y * 16 + x] = x | (y << 8) | (0x77 << 16) | (0x80u << 24);
    upload(0x3000, 1, GS_PSMCT32, 0, 0, 16, 16, clut32, sizeof(clut32));
    for (i = 0; i < 256; i++)
        idx8[i] = (uint8_t)i;
    upload(0x3100, 1, GS_PSMT8, 0, 0, 16, 16, idx8, sizeof(idx8));
    set(GS_TEX0_1, tex0(0x3100, 1, GS_PSMT8, 4, 4, 1, 1, 0x3000, GS_PSMCT32, 0, 0, 1));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_UV, uv(0, 0));
    set(GS_XYZ2, xyz(0, 0, 0));
    set(GS_UV, uv(16, 16));
    set(GS_XYZ2, xyz(16, 16, 0));
    for (i = 0; i < 256; i++) {
        /* index i is at CLUT position i with bits 3 and 4 swapped */
        int l = (i & ~0x18) | ((i & 8) << 1) | ((i & 16) >> 1);
        uint32_t want = clut32[l];
        if (px32(0, i & 15, i >> 4) != want)
            bad++;
    }
    CHECK(bad == 0, "T8 CSM1 CLUT: %d entries wrong", bad);
    CHECK(px32(0, 8, 0) == clut32[16], "index 8 is CLUT pixel (0,1): %08X", px32(0, 8, 0));
    CHECK(px32(0, 0, 1) == clut32[8], "index 16 is CLUT pixel (8,0)");

    /* 4-bit with CSA = 1 (CT32: entries 16-31 of the buffer): the CLUT
       loaded from an 8x2 image */
    for (i = 0; i < 16; i++)
        clut32_4[i] = 0xFF000000u | (uint32_t)(i * 16) | ((uint32_t)(255 - i * 16) << 8);
    upload(0x3200, 1, GS_PSMCT32, 0, 0, 8, 2, clut32_4, 16 * 4);
    for (i = 0; i < 128; i++)
        idx4[i] = (uint8_t)(((2 * i) & 15) | ((((2 * i) + 1) & 15) << 4));
    upload(0x3300, 1, GS_PSMT4, 0, 0, 16, 16, idx4, sizeof(idx4));
    set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3200, GS_PSMCT32, 0, 1, 1));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_UV, uv(0, 0));
    set(GS_XYZ2, xyz(32, 0, 0));
    set(GS_UV, uv(16, 16));
    set(GS_XYZ2, xyz(48, 16, 0));
    bad = 0;
    for (i = 0; i < 16; i++)
        if (px32(0, 32 + i, 3) != clut32_4[i])
            bad++;
    CHECK(bad == 0, "T4 CLUT with CSA: %d wrong", bad);
    CHECK(gs.clut[16 + 5] == (clut32_4[5] & 0xFFFF) && gs.clut[256 + 16 + 5] == clut32_4[5] >> 16,
          "CT32 CLUT halves at CSA 1");

    /* CT16 CLUT, TEXA alpha (TA0 = 0x80 for bit 15 = 0, TA1 = 0x40), AEM */
    for (i = 0; i < 16; i++)
        clut16[i] = (uint16_t)(i | (i == 3 ? 0x8000 : 0));
    clut16[0] = 0;
    upload(0x3400, 1, GS_PSMCT16, 0, 0, 8, 2, clut16, sizeof(clut16));
    set(GS_TEXA, 0x80ull | (1ull << 15) | (0x40ull << 32));
    set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3400, GS_PSMCT16, 0, 0, 1));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    set(GS_UV, uv(0, 0));
    set(GS_XYZ2, xyz(64, 0, 0));
    set(GS_UV, uv(16, 16));
    set(GS_XYZ2, xyz(80, 16, 0));
    CHECK(px32(0, 64 + 0, 0) == 0x00000000u, "AEM black: %08X", px32(0, 64, 0));
    CHECK(px32(0, 64 + 3, 0) == (0x40000000u | (3 << 3)), "TA1: %08X", px32(0, 67, 0));
    CHECK(px32(0, 64 + 5, 0) == (0x80000000u | (5 << 3)), "TA0: %08X", px32(0, 69, 0));

    /* CSM2: a CT16 line at (COU*16, COV) of a buffer CBW wide */
    {
        uint16_t line[16];
        for (i = 0; i < 16; i++)
            line[i] = (uint16_t)(0x8000 | (i << 5));
        upload(0x3800, 4, GS_PSMCT16, 32, 7, 16, 1, line, sizeof(line));
        set(GS_TEXCLUT, 4 | (2 << 6) | (7 << 12));
        set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3800, GS_PSMCT16, 1, 0, 1));
        set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
        set(GS_UV, uv(0, 0));
        set(GS_XYZ2, xyz(96, 0, 0));
        set(GS_UV, uv(16, 16));
        set(GS_XYZ2, xyz(112, 16, 0));
        CHECK(px32(0, 96 + 9, 0) == (0x40000000u | ((9u << 3) << 8)), "CSM2: %08X", px32(0, 105, 0));
    }
    /* CLD 4: no reload when CBP equals CBP0 */
    set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3200, GS_PSMCT32, 0, 0, 2));
    gs.clut[0] = 0xBEEF;
    set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3200, GS_PSMCT32, 0, 0, 4));
    CHECK(gs.clut[0] == 0xBEEF, "CLD 4 skipped the load");
    set(GS_TEX0_1, tex0(0x3300, 1, GS_PSMT4, 4, 4, 1, 1, 0x3400, GS_PSMCT32, 0, 0, 4));
    CHECK(gs.clut[0] != 0xBEEF, "CLD 4 loaded a new CBP");
    set(GS_TEXA, 0x80ull | (0x80ull << 32));
    snapshot("clut", GS_PSMCT32, 0, 128, 32);
}

/* Gouraud triangle, coverage of a shared edge, PACKED vertices */
static void test_triangles(void) {
    int x, y, bad = 0, over = 0, holes = 0;
    uint32_t c;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    /* PACKED: PRIM via PRE, then RGBAQ XYZ2 x3 */
    tag(3, 1, 1, PRIM(3, 1, 0, 0, 0, 0), 0, 2, 0x51);
    q(255, 0 | (128ull << 32));                    /* RGBAQ red */
    q(10 * 16 | ((uint64_t)(10 * 16) << 32), 0);   /* XYZ2 */
    q(0 | (255ull << 32), 0 | (128ull << 32));     /* green */
    q(210 * 16 | ((uint64_t)(10 * 16) << 32), 0);
    q(0, 255 | (128ull << 32));                    /* blue */
    q(10 * 16 | ((uint64_t)(210 * 16) << 32), 0);
    flush();
    c = px32(0, 10, 10);
    CHECK((c & 0xFFFFFF) == 0x000000FFu, "red corner: %08X", c);
    c = px32(0, 209, 10);
    CHECK((c & 255) <= 2 && ((c >> 8) & 255) >= 252, "green corner: %08X", c);
    c = px32(0, 10, 209);
    CHECK(((c >> 16) & 255) >= 252, "blue corner: %08X", c);
    c = px32(0, 110, 60);
    CHECK((c & 255) > 40 && ((c >> 8) & 255) > 120 && ((c >> 16) & 255) > 40, "middle: %08X", c);
    CHECK(px32(0, 210, 10) == 0 && px32(0, 10, 210) == 0, "right and bottom exclusive");
    CHECK(px32(0, 110, 110) == 0, "the hypotenuse's own pixel (110,110) is out (right edge)");
    CHECK(px32(0, 109, 110) != 0, "(109,110) in");
    snapshot("gouraud", GS_PSMCT32, 0, 220, 220);

    /* Two triangles sharing a diagonal, added with FIX: every pixel once */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_ALPHA_1, 0 | (2 << 2) | (2 << 4) | (1 << 6) | (128ull << 32)); /* Cs*FIX + Cd */
    set(GS_PRIM, PRIM(4, 0, 0, 0, 1, 0)); /* strip */
    set(GS_RGBAQ, rgba(50, 0, 0, 128));
    /* odd sub-pixel positions */
    set(GS_XYZ2, (uint64_t)(300 * 16 + 3) | ((uint64_t)(20 * 16 + 7) << 16));
    set(GS_XYZ2, (uint64_t)(400 * 16 + 11) | ((uint64_t)(25 * 16 + 1) << 16));
    set(GS_XYZ2, (uint64_t)(305 * 16 + 9) | ((uint64_t)(130 * 16 + 13) << 16));
    set(GS_XYZ2, (uint64_t)(410 * 16 + 5) | ((uint64_t)(120 * 16 + 2) << 16));
    for (y = 15; y < 140; y++)
        for (x = 290; x < 420; x++) {
            uint32_t r = px32(0, x, y) & 255;
            if (r > 50)
                over++;
            else if (r == 0 && x > 310 && x < 395 && y > 30 && y < 115)
                holes++;
        }
    CHECK(over == 0 && holes == 0, "shared edge: %d drawn twice, %d holes", over, holes);
    set(GS_ALPHA_1, 0);

    /* A fan and a strip: vertex queue */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_PRIM, PRIM(5, 0, 0, 0, 0, 0));
    set(GS_RGBAQ, rgba(0, 0, 200, 128));
    set(GS_XYZ2, xyz(500, 100, 0));
    set(GS_XYZ2, xyz(600, 100, 0));
    set(GS_XYZ2, xyz(600, 200, 0));
    set(GS_XYZ2, xyz(500, 200, 0));
    CHECK(px32(0, 590, 110) == 0x80C80000u && px32(0, 510, 190) == 0x80C80000u, "fan");
    /* XYZ3 kicks no drawing */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_PRIM, PRIM(3, 0, 0, 0, 0, 0));
    set(GS_XYZ2, xyz(500, 100, 0));
    set(GS_XYZ2, xyz(600, 100, 0));
    set(GS_XYZ3, xyz(600, 200, 0));
    CHECK(px32(0, 590, 110) == 0, "XYZ3 does not draw");

    /* A perspective floor: STQ, a checker of 8x8 squares. Affine
       mapping would put square row 4 at (320, 300); perspective gives
       t = 0.2 there (row 1) */
    {
        static uint32_t chk[64 * 64];
        for (y = 0; y < 64; y++)
            for (x = 0; x < 64; x++)
                chk[y * 64 + x] = (((x >> 3) ^ (y >> 3)) & 1) ? 0x80FFFFFFu : 0x80202020u;
        upload(0x2800, 1, GS_PSMCT32, 0, 0, 64, 64, chk, sizeof(chk));
        fill(0, 0, 640, 448, 0, 0, 0, 0);
        set(GS_TEX0_1, tex0(0x2800, 1, GS_PSMCT32, 6, 6, 1, 1, 0, 0, 0, 0, 0));
        set(GS_PRIM, PRIM(4, 0, 1, 0, 0, 0));
        set(GS_ST, 0);
        set(GS_RGBAQ, rgba(128, 128, 128, 128) & 0xFFFFFFFFull);
        set(GS_RGBAQ, rgba(128, 128, 128, 128) | (0x3F800000ull << 32));
        set(GS_XYZ2, xyz(100, 400, 0));
        set(GS_ST, 0x3F800000ull);
        set(GS_XYZ2, xyz(540, 400, 0));
        set(GS_ST, 0x3E800000ull << 32); /* S 0, T 0.25 */
        set(GS_RGBAQ, (uint32_t)rgba(128, 128, 128, 128) | (0x3E800000ull << 32));
        set(GS_XYZ2, xyz(250, 200, 0));
        set(GS_ST, 0x3E800000ull | (0x3E800000ull << 32));
        set(GS_XYZ2, xyz(390, 200, 0));
        CHECK(px32(0, 320, 300) == 0x80FFFFFFu, "perspective (320,300): %08X", px32(0, 320, 300));
        CHECK(px32(0, 320, 380) == 0x80202020u, "perspective (320,380): %08X", px32(0, 320, 380));
        snapshot("perspective", GS_PSMCT32, 0, 640, 448);
    }

    /* Lines and points */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_PRIM, PRIM(1, 0, 0, 0, 0, 0));
    set(GS_RGBAQ, rgba(255, 255, 255, 128));
    set(GS_XYZ2, xyz(10, 300, 0));
    set(GS_XYZ2, xyz(110, 300, 0));
    bad = 0;
    for (x = 10; x < 110; x++)
        if (px32(0, x, 300) != 0x80FFFFFFu)
            bad++;
    CHECK(bad == 0 && px32(0, 110, 300) == 0, "horizontal line: %d missing", bad);
    set(GS_PRIM, PRIM(0, 0, 0, 0, 0, 0));
    set(GS_XYZ2, xyz(200, 300, 0));
    CHECK(px32(0, 200, 300) == 0x80FFFFFFu, "point");
}

static void test_blend_scissor_z(void) {
    uint32_t c;
    int x, y, bad;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 100, 100, 100, 0x80);
    /* (Cs - Cd) * As >> 7 + Cd with As = 0x40: 100 + (200 - 100) * 64 / 128 = 150 */
    set(GS_ALPHA_1, 0 | (1 << 2) | (0 << 4) | (1 << 6));
    set(GS_PRIM, PRIM(6, 0, 0, 0, 1, 0));
    set(GS_RGBAQ, rgba(200, 0, 255, 0x40));
    set(GS_XYZ2, xyz(10, 10, 0));
    set(GS_XYZ2, xyz(20, 20, 0));
    c = px32(0, 15, 15);
    CHECK(c == (150u | (50u << 8) | (177u << 16) | (0x40u << 24)), "blend: %08X", c);
    /* COLCLAMP off wraps: Cs + Cd with FIX 128 */
    set(GS_COLCLAMP, 0);
    set(GS_ALPHA_1, 0 | (2 << 2) | (2 << 4) | (1 << 6) | (128ull << 32));
    set(GS_RGBAQ, rgba(200, 0, 0, 0x80));
    set(GS_XYZ2, xyz(30, 10, 0));
    set(GS_XYZ2, xyz(40, 20, 0));
    CHECK((px32(0, 35, 15) & 255) == ((100 + 200) & 255), "no colclamp wraps: %08X", px32(0, 35, 15));
    set(GS_COLCLAMP, 1);
    set(GS_XYZ2, xyz(50, 10, 0));
    set(GS_XYZ2, xyz(60, 20, 0));
    CHECK((px32(0, 55, 15) & 255) == 255, "colclamp clamps");
    /* PABE: blending only where As bit 7 is set */
    set(GS_PABE, 1);
    set(GS_ALPHA_1, 0 | (1 << 2) | (0 << 4) | (1 << 6));
    set(GS_RGBAQ, rgba(200, 0, 0, 0x40));
    set(GS_XYZ2, xyz(70, 10, 0));
    set(GS_XYZ2, xyz(80, 20, 0));
    CHECK((px32(0, 75, 15) & 255) == 200, "PABE skips the blend");
    set(GS_PABE, 0);
    set(GS_ALPHA_1, 0);

    /* Scissor */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_SCISSOR_1, scissor(100, 199, 50, 149));
    fill(0, 0, 640, 448, 9, 9, 9, 9);
    bad = 0;
    for (y = 40; y < 160; y++)
        for (x = 90; x < 210; x++) {
            int in = x >= 100 && x <= 199 && y >= 50 && y <= 149;
            if ((px32(0, x, y) != 0) != in)
                bad++;
        }
    CHECK(bad == 0, "scissor: %d pixels wrong", bad);
    set(GS_SCISSOR_1, scissor(0, 639, 0, 447));

    /* Z: Z32 buffer cleared to 0, near (big Z) then far quad with GEQUAL */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_ZBUF_1, 0x100 | ((uint64_t)0 << 24));
    set(GS_TEST_1, (1u << 16) | (1u << 17)); /* ZTE, ALWAYS */
    set(GS_FRAME_1, frame(0, 10, GS_PSMCT32, 0xFFFFFFFFu)); /* clear Z only */
    set(GS_PRIM, PRIM(6, 0, 0, 0, 0, 0));
    set(GS_XYZ2, xyz(0, 0, 0));
    set(GS_XYZ2, xyz(640, 448, 0));
    set(GS_FRAME_1, frame(0, 10, GS_PSMCT32, 0));
    set(GS_TEST_1, (1u << 16) | (2u << 17)); /* GEQUAL */
    set(GS_RGBAQ, rgba(255, 0, 0, 0x80));
    set(GS_XYZ2, xyz(100, 100, 5000));
    set(GS_XYZ2, xyz(200, 200, 5000));
    set(GS_RGBAQ, rgba(0, 255, 0, 0x80));
    set(GS_XYZ2, xyz(150, 150, 1000));
    set(GS_XYZ2, xyz(250, 250, 1000));
    CHECK(px32(0, 160, 160) == 0x800000FFu, "near stays in front: %08X", px32(0, 160, 160));
    CHECK(px32(0, 220, 220) == 0x8000FF00u, "far drawn where empty");
    CHECK(gs_read_pixel(GS_PSMZ32, 0x100 * 32, 10, 160, 160) == 5000, "Z written: %u",
          gs_read_pixel(GS_PSMZ32, 0x100 * 32, 10, 160, 160));
    /* GREATER with equal Z fails */
    set(GS_TEST_1, (1u << 16) | (3u << 17));
    set(GS_RGBAQ, rgba(0, 0, 255, 0x80));
    set(GS_XYZ2, xyz(100, 100, 5000));
    set(GS_XYZ2, xyz(120, 120, 5000));
    CHECK(px32(0, 110, 110) == 0x800000FFu, "GREATER fails on equal");
    /* Z mask: test but no write */
    set(GS_ZBUF_1, 0x100 | (1ull << 32));
    set(GS_TEST_1, (1u << 16) | (2u << 17));
    set(GS_XYZ2, xyz(300, 300, 9000));
    set(GS_XYZ2, xyz(310, 310, 9000));
    CHECK(gs_read_pixel(GS_PSMZ32, 0x100 * 32, 10, 305, 305) == 0, "ZMSK");
    /* Gouraud Z on a triangle in Z24 */
    set(GS_ZBUF_1, 0x100 | (1ull << 24));
    set(GS_TEST_1, (1u << 16) | (1u << 17));
    set(GS_PRIM, PRIM(3, 0, 0, 0, 0, 0));
    set(GS_XYZ2, xyz(400, 0, 0));
    set(GS_XYZ2, xyz(500, 0, 0x1000000 + 5)); /* clamped to 0xFFFFFF */
    set(GS_XYZ2, xyz(400, 100, 0));
    CHECK(gs_read_pixel(GS_PSMZ24, 0x100 * 32, 10, 450, 0) >= 0x7F0000 &&
              gs_read_pixel(GS_PSMZ24, 0x100 * 32, 10, 450, 0) <= 0x810000,
          "Z interpolated: %06X", gs_read_pixel(GS_PSMZ24, 0x100 * 32, 10, 450, 0));
    set(GS_TEST_1, 0);

    /* Alpha test: GEQUAL 0x40, AFAIL KEEP; then FB_ONLY with Z */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_TEST_1, 1 | (5 << 1) | (0x40 << 4) | (0 << 12));
    set(GS_PRIM, PRIM(6, 0, 0, 0, 0, 0));
    set(GS_RGBAQ, rgba(255, 255, 255, 0x3F));
    set(GS_XYZ2, xyz(0, 0, 0));
    set(GS_XYZ2, xyz(10, 10, 0));
    set(GS_RGBAQ, rgba(255, 255, 255, 0x40));
    set(GS_XYZ2, xyz(10, 0, 0));
    set(GS_XYZ2, xyz(20, 10, 0));
    CHECK(px32(0, 5, 5) == 0 && px32(0, 15, 5) == 0x40FFFFFFu, "alpha test");
    /* RGB_ONLY keeps the destination alpha */
    set(GS_TEST_1, 0);
    fill(0, 0, 640, 448, 0, 0, 0, 0x22);
    set(GS_TEST_1, 1 | (0 << 1) | (3 << 12));
    set(GS_RGBAQ, rgba(1, 2, 3, 0x77));
    set(GS_XYZ2, xyz(0, 0, 0));
    set(GS_XYZ2, xyz(10, 10, 0));
    CHECK(px32(0, 5, 5) == 0x22030201u, "AFAIL RGB_ONLY: %08X", px32(0, 5, 5));
    /* Destination alpha test: draw only where dst alpha bit is 1 (DATM 1) */
    set(GS_TEST_1, 0);
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    fill(0, 0, 5, 10, 0, 0, 0, 0x80);
    set(GS_TEST_1, (1 << 14) | (1 << 15));
    fill(0, 0, 10, 10, 9, 9, 9, 0x80);
    CHECK(px32(0, 2, 2) == 0x80090909u && px32(0, 7, 2) == 0, "DATE");
    set(GS_TEST_1, 0);

    /* FBMSK: only green written */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_FRAME_1, frame(0, 10, GS_PSMCT32, 0xFFFF00FFu));
    fill(0, 0, 10, 10, 255, 255, 255, 255);
    CHECK(px32(0, 1, 1) == 0x0000FF00u, "FBMSK");
    set(GS_FRAME_1, frame(0, 10, GS_PSMCT32, 0));

    /* Fog: F = 0 is all fog colour */
    set(GS_FOGCOL, 0x102030);
    set(GS_PRIM, PRIM(6, 0, 0, 1, 0, 0));
    set(GS_RGBAQ, rgba(255, 255, 255, 0x80));
    set(GS_XYZF2, xyz(0, 0, 0) | (0ull << 56));
    set(GS_XYZF2, xyz(10, 10, 0) | (0ull << 56));
    c = px32(0, 3, 3);
    CHECK(c == (0x80000000u | (0x10 * 255 / 256) << 16 | (0x20 * 255 / 256) << 8 | (0x30 * 255 / 256)), "fog: %08X", c);

    /* 16-bit frame with dithering */
    basic_state(GS_PSMCT16);
    set(GS_DTHE, 1);
    set(GS_DIMX, 3); /* DM00 = 3, the rest 0 */
    fill(0, 0, 16, 16, 0x84, 0x80, 0x80, 0x80);
    CHECK(gs_read_pixel(GS_PSMCT16, 0, 10, 0, 0) == (0x8000 | (16 << 10) | (16 << 5) | 16) &&
              gs_read_pixel(GS_PSMCT16, 0, 10, 1, 0) == (0x8000 | (16 << 10) | (16 << 5) | 16),
          "16-bit write: %04X", gs_read_pixel(GS_PSMCT16, 0, 10, 0, 0));
    fill(0, 0, 16, 16, 0x85, 0x80, 0x80, 0x80);
    CHECK((gs_read_pixel(GS_PSMCT16, 0, 10, 0, 0) & 31) == 17 && (gs_read_pixel(GS_PSMCT16, 0, 10, 1, 0) & 31) == 16,
          "dither");
    set(GS_DTHE, 0);
}

/* REGLIST, data split over several calls, HWREG, wrap modes, XYOFFSET */
static void test_gif_modes(void) {
    static uint32_t img[40 * 10], tex[16 * 16];
    uint32_t i, start, x, y;
    int bad = 0;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    /* REGLIST: PRIM RGBAQ XYZ2 XYZ2 (an odd count of 4 x 1... and NREG 3
       with NLOOP 1 for the padding case) */
    tag(1, 0, 0, 0, 1, 4, 0x5510ull);
    q(PRIM(6, 0, 0, 0, 0, 0), rgba(1, 2, 3, 4));
    q(xyz(10, 10, 0), xyz(20, 20, 0));
    tag(1, 0, 0, 0, 1, 3, 0x551ull); /* RGBAQ XYZ2 XYZ2 + padding */
    q(rgba(5, 6, 7, 8), xyz(30, 10, 0));
    q(xyz(40, 20, 0), 0xDEADBEEFull);
    flush();
    CHECK(px32(0, 15, 15) == 0x04030201u && px32(0, 35, 15) == 0x08070605u, "REGLIST: %08X %08X",
          px32(0, 15, 15), px32(0, 35, 15));

    /* An IMAGE upload, and a PACKED packet, split at odd points */
    for (i = 0; i < 40 * 10; i++)
        img[i] = rnd();
    set(GS_BITBLTBUF, (0x2400ull << 32) | (1ull << 48));
    set(GS_TRXPOS, 0);
    set(GS_TRXREG, 40 | (10ull << 32));
    set(GS_TRXDIR, 0);
    tag(100, 1, 0, 0, 2, 0, 0);
    memcpy(&pkt[np], img, sizeof(img));
    np += 200;
    tag(1, 1, 1, PRIM(6, 0, 0, 0, 0, 0), 0, 3, 0x551ull);
    q(9 | (9ull << 32), 9 | (9ull << 32));
    q(50 * 16 | ((uint64_t)(10 * 16) << 32), 0);
    q(60 * 16 | ((uint64_t)(20 * 16) << 32), 0);
    for (start = 0, i = 0; start < np / 2; i++) {
        uint32_t n = 1 + (i * 7) % 13;
        if (start + n > np / 2)
            n = np / 2 - start;
        grx_gs_gif(pkt + start * 2, n, GRX_GIF_PATH3);
        start += n;
    }
    np = 0;
    gs_flush();
    for (i = 0; i < 40 * 10; i++)
        if (gs_read_pixel(GS_PSMCT32, 0x2400, 1, i % 40, i / 40) != img[i])
            bad++;
    CHECK(bad == 0, "split IMAGE: %d wrong", bad);
    CHECK(px32(0, 55, 15) == 0x09090909u, "split PACKED");

    /* HWREG through A+D: two 32-bit pixels per write */
    set(GS_TRXREG, 2 | (1ull << 32));
    set(GS_TRXDIR, 0);
    set(GS_HWREG, 0x1111111122222222ull);
    CHECK(gs_read_pixel(GS_PSMCT32, 0x2400, 1, 0, 0) == 0x22222222u &&
              gs_read_pixel(GS_PSMCT32, 0x2400, 1, 1, 0) == 0x11111111u,
          "HWREG");

    /* Wrap modes on a 16x16 texture: texel (x, y) = x | y << 8 */
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++)
            tex[y * 16 + x] = x | (y << 8) | 0x80000000u;
    upload(0x2600, 1, GS_PSMCT32, 0, 0, 16, 16, tex, sizeof(tex));
    set(GS_TEX0_1, tex0(0x2600, 1, GS_PSMCT32, 4, 4, 1, 1, 0, 0, 0, 0, 0));
    set(GS_PRIM, PRIM(6, 0, 1, 0, 0, 1));
    /* REPEAT: u 20 -> 4 */
    set(GS_CLAMP_1, 0);
    set(GS_UV, uv(20, 3));
    set(GS_XYZ2, xyz(100, 100, 0));
    set(GS_UV, uv(21, 4));
    set(GS_XYZ2, xyz(101, 101, 0));
    CHECK(px32(0, 100, 100) == (0x80000000u | 4 | (3 << 8)), "REPEAT: %08X", px32(0, 100, 100));
    /* CLAMP: u 20 -> 15 */
    set(GS_CLAMP_1, 1 | (1 << 2));
    set(GS_UV, uv(20, 3));
    set(GS_XYZ2, xyz(100, 100, 0));
    set(GS_UV, uv(21, 4));
    set(GS_XYZ2, xyz(101, 101, 0));
    CHECK(px32(0, 100, 100) == (0x80000000u | 15 | (3 << 8)), "CLAMP: %08X", px32(0, 100, 100));
    /* REGION_CLAMP u in [2, 5]; REGION_REPEAT v: (v & 3) | 8 */
    set(GS_CLAMP_1, 2 | (3 << 2) | (2 << 4) | (5 << 14) | (3ull << 24) | (8ull << 34));
    set(GS_UV, uv(9, 6));
    set(GS_XYZ2, xyz(100, 100, 0));
    set(GS_UV, uv(10, 7));
    set(GS_XYZ2, xyz(101, 101, 0));
    CHECK(px32(0, 100, 100) == (0x80000000u | 5 | (10 << 8)), "REGION modes: %08X", px32(0, 100, 100));
    set(GS_CLAMP_1, 0);

    /* XYOFFSET (2048 - 320, 2048 - 224), a sprite partly off the top left */
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_XYOFFSET_1, (uint64_t)((2048 - 320) * 16) | ((uint64_t)((2048 - 224) * 16) << 32));
    set(GS_PRIM, PRIM(6, 0, 0, 0, 0, 0));
    set(GS_RGBAQ, rgba(7, 7, 7, 7));
    set(GS_XYZ2, (uint64_t)((2048 - 330) * 16) | ((uint64_t)((2048 - 230) * 16) << 16));
    set(GS_XYZ2, (uint64_t)((2048 - 310) * 16) | ((uint64_t)((2048 - 214) * 16) << 16));
    CHECK(px32(0, 0, 0) == 0x07070707u && px32(0, 9, 9) == 0x07070707u && px32(0, 10, 10) == 0, "XYOFFSET");
    set(GS_XYOFFSET_1, 0);
}

static void test_display_and_csr(void) {
    const uint32_t *pix;
    int w, h, pitch;
    uint64_t v;

    basic_state(GS_PSMCT32);
    fill(0, 0, 640, 448, 0x10, 0x20, 0x30, 0x80);
    fill(0, 0, 1, 1, 0xFF, 0, 0, 0x80);
    fill(639, 447, 640, 448, 0, 0, 0xFF, 0x80);
    /* sceGsSetDefDispEnv-like: circuit 2, NTSC 640x448, MAGH 3 */
    v = (1u << 1) | (1u << 5); /* EN2, MMOD, ALP 0 */
    memcpy(priv + GS_PRIV_PMODE, &v, 8);
    v = 0 | (10u << 9) | (0u << 15);
    memcpy(priv + GS_PRIV_DISPFB2, &v, 8);
    v = 636 | (50u << 12) | (3u << 23) | ((uint64_t)2559 << 32) | ((uint64_t)447 << 44);
    memcpy(priv + GS_PRIV_DISPLAY2, &v, 8);
    CHECK(grx_gs_display(&pix, &w, &h, &pitch) == 1, "display on");
    CHECK(w == 640 && h == 448, "display size %dx%d", w, h);
    CHECK(pix[0] == 0xFFFF0000u && pix[447 * pitch + 639] == 0xFF0000FFu && pix[100 * pitch + 100] == 0xFF102030u,
          "display pixels %08X %08X %08X", pix[0], pix[447 * pitch + 639], pix[100 * pitch + 100]);
    /* Circuit 1 over circuit 2 with ALP 0x80 */
    v = 1 | (1u << 1) | (1u << 5) | (0x80u << 8);
    memcpy(priv + GS_PRIV_PMODE, &v, 8);
    v = 0 | (10u << 9);
    memcpy(priv + GS_PRIV_DISPFB1, &v, 8);
    v = 636 | (50u << 12) | (3u << 23) | ((uint64_t)2559 << 32) | ((uint64_t)447 << 44);
    memcpy(priv + GS_PRIV_DISPLAY1, &v, 8);
    v = 0xFFFFFF;
    memcpy(priv + GS_PRIV_BGCOLOR, &v, 8);
    CHECK(grx_gs_display(&pix, &w, &h, &pitch) == 1 && pix[100 * pitch + 100] == 0xFF102030u, "merge same: %08X",
          pix[100 * pitch + 100]);
    v = 1 | (1u << 1) | (1u << 5) | (1u << 7) | (0x80u << 8); /* SLBG: blend with BGCOLOR */
    memcpy(priv + GS_PRIV_PMODE, &v, 8);
    grx_gs_display(&pix, &w, &h, &pitch);
    {
        uint32_t p = pix[100 * pitch + 100];
        uint32_t r = (p >> 16) & 255;
        CHECK(r >= 0x86 && r <= 0x89, "merge with background: %08X", p);
    }
    v = 0;
    memcpy(priv + GS_PRIV_PMODE, &v, 8);
    CHECK(grx_gs_display(&pix, &w, &h, &pitch) == 0, "display off");

    /* CSR */
    memset(priv + GS_PRIV_CSR, 0, 8);
    set(GS_SIGNAL, 0x1234 | (0xFFFFull << 32));
    set(GS_FINISH, 0);
    set(GS_LABEL, 0xAB | (0xFFull << 32));
    memcpy(&v, priv + GS_PRIV_CSR, 8);
    CHECK((v & 3) == 3, "SIGNAL and FINISH in CSR: %llX", (unsigned long long)v);
    memcpy(&v, priv + GS_PRIV_SIGLBLID, 8);
    CHECK(v == (0x1234 | (0xABull << 32)), "SIGLBLID %llX", (unsigned long long)v);
    memcpy(&v, priv + GS_PRIV_CSR, 8);
    grx_gs_vsync();
    {
        uint64_t v2;
        memcpy(&v2, priv + GS_PRIV_CSR, 8);
        CHECK(((v ^ v2) & (1u << 13)) && (v2 & 8), "vsync toggles FIELD and sets VSINT");
    }
}

/* ---- performance ---- */

/* Replays the packet built in pkt several times; the best time */
static double bench(const char *what, double pixels, int reps) {
    uint32_t n = np;
    double best = 1e9;
    int i;
    for (i = 0; i < reps; i++) {
        clock_t t0 = clock();
        grx_gs_gif(pkt, n / 2, GRX_GIF_PATH3);
        gs_flush();
        double s = (double)(clock() - t0) / CLOCKS_PER_SEC;
        if (s < best)
            best = s;
    }
    np = 0;
    if (best <= 0)
        best = 1e-4;
    printf("perf: %-48s %8.2f ms  %6.1f Mpixels/s\n", what, best * 1000, pixels / best / 1e6);
    return best;
}

static void sprites(int n, int sz, uint64_t prim, uint32_t rgba_a) {
    int i;
    for (i = 0; i < n; i++) {
        int x = (int)(rnd() % (640 - sz)), y = (int)(rnd() % (448 - sz));
        tag(1, 0, 1, prim, 0, 5, 0x53531ull);
        q(128 | (128ull << 32), 128 | ((uint64_t)rgba_a << 32)); /* RGBAQ */
        q(0, 0);                                                    /* UV */
        q(x * 16 | ((uint64_t)(y * 16) << 32), 0);                  /* XYZ2 */
        q((sz * 16) | ((uint64_t)(sz * 16) << 32), 0);              /* UV */
        q((x + sz) * 16 | ((uint64_t)((y + sz) * 16) << 32), 0);    /* XYZ2 */
    }
}

static void perf(void) {
    static uint32_t tex[256 * 256];
    static uint8_t idx[256 * 256];
    int i, n;
    double t_bg, t_text;

    basic_state(GS_PSMCT32);
    for (i = 0; i < 256 * 256; i++) {
        tex[i] = rnd() | 0x80000000u;
        idx[i] = (uint8_t)rnd();
    }
    upload(0x2000, 4, GS_PSMCT32, 0, 0, 256, 256, tex, sizeof(tex));
    upload(0x3000, 4, GS_PSMT8, 0, 0, 256, 256, idx, sizeof(idx));
    upload(0x3800, 1, GS_PSMCT32, 0, 0, 16, 16, tex, 1024);

    for (i = 0; i < 20; i++) {
        tag(1, 0, 1, PRIM(6, 0, 0, 0, 0, 0), 0, 2, 0x51ull);
        q(i, 0x80ull << 32);
        q(0, 0);
        tag(1, 0, 0, 0, 0, 1, 0x5ull);
        q(640 * 16 | ((uint64_t)(448 * 16) << 32), 0);
    }
    bench("20 flat full-screen fills", 20 * 640.0 * 448, 5);

    set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMCT32, 8, 8, 1, 0, 0, 0, 0, 0, 0));
    sprites(2000, 32, PRIM(6, 0, 1, 0, 0, 1), 0x80);
    bench("2000 CT32 textured 32x32 sprites", 2000 * 1024.0, 5);

    set(GS_TEX0_1, tex0(0x3000, 4, GS_PSMT8, 8, 8, 1, 0, 0x3800, GS_PSMCT32, 0, 0, 1));
    sprites(2000, 32, PRIM(6, 0, 1, 0, 0, 1), 0x80);
    bench("2000 T8 CLUT textured 32x32 sprites", 2000 * 1024.0, 5);

    set(GS_ALPHA_1, 0 | (1 << 2) | (0 << 4) | (1 << 6));
    set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMCT32, 8, 8, 1, 0, 0, 0, 0, 0, 0));
    set(GS_TEX1_1, (1u << 5) | (1u << 6));
    sprites(2000, 32, PRIM(6, 0, 1, 0, 1, 1), 0x40);
    bench("2000 blended bilinear 32x32 sprites", 2000 * 1024.0, 5);
    set(GS_TEX1_1, 0);
    set(GS_ALPHA_1, 0);

    /* A menu-like frame: a 640x448 textured background, then 1500 blended
       16x16 glyphs from a T8 CLUT texture */
    set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMCT32, 8, 8, 1, 1, 0, 0, 0, 0, 0));
    tag(1, 0, 1, PRIM(6, 0, 1, 0, 0, 1), 0, 4, 0x5353ull);
    q(0, 0);
    q(0, 0);
    q(256 * 16 | ((uint64_t)(256 * 16) << 32), 0);
    q(640 * 16 | ((uint64_t)(448 * 16) << 32), 0);
    t_bg = bench("640x448 textured background sprite", 640.0 * 448, 5);
    set(GS_ALPHA_1, 0 | (1 << 2) | (0 << 4) | (1 << 6));
    set(GS_TEX0_1, tex0(0x3000, 4, GS_PSMT8, 8, 8, 1, 0, 0x3800, GS_PSMCT32, 0, 0, 1));
    sprites(1500, 16, PRIM(6, 0, 1, 0, 1, 1), 0x80);
    t_text = bench("1500 blended T8 16x16 glyphs", 1500 * 256.0, 5);
    printf("perf: menu-like frame (background + 1500 glyphs): %.2f ms\n", (t_bg + t_text) * 1000);
    set(GS_ALPHA_1, 0);

    /* Gouraud Z-tested triangles, ~1000 pixels each, over a cleared Z */
    set(GS_ZBUF_1, 0x100);
    set(GS_TEST_1, (1u << 16) | (1u << 17));
    fill(0, 0, 640, 448, 0, 0, 0, 0);
    set(GS_TEST_1, (1u << 16) | (2u << 17));
    n = 5000;
    for (i = 0; i < n; i++) {
        int x = (int)(rnd() % 590), y = (int)(rnd() % 400);
        uint32_t z = rnd() & 0xFFFFFF;
        tag(3, 0, 1, PRIM(3, 1, 0, 0, 0, 0), 0, 2, 0x51);
        q((rnd() & 255) | ((uint64_t)(rnd() & 255) << 32), (rnd() & 255) | (128ull << 32));
        q(x * 16 | ((uint64_t)(y * 16) << 32), z + i * 4000);
        q((rnd() & 255) | ((uint64_t)(rnd() & 255) << 32), (rnd() & 255) | (128ull << 32));
        q((x + 45) * 16 | ((uint64_t)(y * 16) << 32), z + i * 4000);
        q((rnd() & 255) | ((uint64_t)(rnd() & 255) << 32), (rnd() & 255) | (128ull << 32));
        q(x * 16 | ((uint64_t)((y + 45) * 16) << 32), z + i * 4000);
    }
    bench("5000 Gouraud Z triangles (~1000 px)", n * 1012.0, 1);

    /* Perspective-textured Gouraud triangles, 64x64 */
    set(GS_TEST_1, 0);
    set(GS_TEX0_1, tex0(0x2000, 4, GS_PSMCT32, 8, 8, 1, 0, 0, 0, 0, 0, 0));
    n = 3000;
    for (i = 0; i < n; i++) {
        int x = (int)(rnd() % 570), y = (int)(rnd() % 380);
        tag(3, 0, 1, PRIM(3, 1, 1, 0, 0, 0), 0, 3, 0x512);
        q(0x3F000000ull, 0x3F80000000000000ull | 0x3F800000); /* ST 0.5 0 Q 1 */
        q(200 | (100ull << 32), 50 | (128ull << 32));
        q(x * 16 | ((uint64_t)(y * 16) << 32), 0);
        q(0x3F800000ull, 0x3F000000);                           /* ST 1 0, Q 0.5 */
        q(100 | (200ull << 32), 50 | (128ull << 32));
        q((x + 64) * 16 | ((uint64_t)(y * 16) << 32), 0);
        q((uint64_t)0x3F800000 << 32, 0x3F400000);              /* ST 0 1, Q 0.75 */
        q(100 | (100ull << 32), 250 | (128ull << 32));
        q(x * 16 | ((uint64_t)((y + 64) * 16) << 32), 0);
    }
    bench("3000 perspective textured triangles (~2000 px)", n * 2048.0, 1);
    snapshot("perf", GS_PSMCT32, 0, 640, 448);
}

int main(int argc, char **argv) {
    if (argc > 1)
        snapdir = argv[1];
    grx_gs_set_priv(priv);
    grx_gs_init();

    test_tables();
    test_roundtrip();
    test_sprite_texture();
    test_clut();
    test_triangles();
    test_blend_scissor_z();
    test_gif_modes();
    test_display_and_csr();
    perf();

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
