/* The software GS's shared state (port/src/gs/). Not for the game: the
   interface is port/include/grx_gs.h. Tests include this too, for
   grx_gs_set_priv() and direct access to local memory.

   Conventions:
   - Colours inside the GS are 32-bit 0xAABBGGRR (R in the low byte), as
     PSMCT32 stores them.
   - Local memory is 4 MB, addressed in 256-byte blocks (BP, as TBP0/CBP/
     BITBLTBUF use), 8 KB pages (FBP, ZBP), and buffer widths in units of
     64 pixels.
   - Vertex X/Y are 12.4 fixed point; pixels are sampled at integer
     positions; a pixel is inside a triangle or sprite when its sample
     point is on or right of the left edge and strictly left of the right
     edge, on or below the top and strictly above the bottom (top-left). */
#ifndef GS_INTERNAL_H
#define GS_INTERNAL_H

#include <stdint.h>
#include <string.h>

#define GS_VRAM_SIZE (4u << 20)

/* Pixel storage modes (TEX0.PSM, FRAME.PSM, BITBLTBUF.xPSM, ...) */
enum {
    GS_PSMCT32 = 0x00,
    GS_PSMCT24 = 0x01,
    GS_PSMCT16 = 0x02,
    GS_PSMCT16S = 0x0A,
    GS_PSMT8 = 0x13,
    GS_PSMT4 = 0x14,
    GS_PSMT8H = 0x1B,
    GS_PSMT4HL = 0x24,
    GS_PSMT4HH = 0x2C,
    GS_PSMZ32 = 0x30,
    GS_PSMZ24 = 0x31,
    GS_PSMZ16 = 0x32,
    GS_PSMZ16S = 0x3A,
};

/* GS register addresses (A+D, REGLIST) */
enum {
    GS_PRIM = 0x00, GS_RGBAQ = 0x01, GS_ST = 0x02, GS_UV = 0x03,
    GS_XYZF2 = 0x04, GS_XYZ2 = 0x05, GS_TEX0_1 = 0x06, GS_TEX0_2 = 0x07,
    GS_CLAMP_1 = 0x08, GS_CLAMP_2 = 0x09, GS_FOG = 0x0A, GS_XYZF3 = 0x0C,
    GS_XYZ3 = 0x0D, GS_TEX1_1 = 0x14, GS_TEX1_2 = 0x15, GS_TEX2_1 = 0x16,
    GS_TEX2_2 = 0x17, GS_XYOFFSET_1 = 0x18, GS_XYOFFSET_2 = 0x19,
    GS_PRMODECONT = 0x1A, GS_PRMODE = 0x1B, GS_TEXCLUT = 0x1C,
    GS_SCANMSK = 0x22, GS_MIPTBP1_1 = 0x34, GS_MIPTBP1_2 = 0x35,
    GS_MIPTBP2_1 = 0x36, GS_MIPTBP2_2 = 0x37, GS_TEXA = 0x3B,
    GS_FOGCOL = 0x3D, GS_TEXFLUSH = 0x3F, GS_SCISSOR_1 = 0x40,
    GS_SCISSOR_2 = 0x41, GS_ALPHA_1 = 0x42, GS_ALPHA_2 = 0x43,
    GS_DIMX = 0x44, GS_DTHE = 0x45, GS_COLCLAMP = 0x46, GS_TEST_1 = 0x47,
    GS_TEST_2 = 0x48, GS_PABE = 0x49, GS_FBA_1 = 0x4A, GS_FBA_2 = 0x4B,
    GS_FRAME_1 = 0x4C, GS_FRAME_2 = 0x4D, GS_ZBUF_1 = 0x4E,
    GS_ZBUF_2 = 0x4F, GS_BITBLTBUF = 0x50, GS_TRXPOS = 0x51,
    GS_TRXREG = 0x52, GS_TRXDIR = 0x53, GS_HWREG = 0x54,
    GS_SIGNAL = 0x60, GS_FINISH = 0x61, GS_LABEL = 0x62,
};

/* Privileged registers: offsets from 0x12000000 */
enum {
    GS_PRIV_PMODE = 0x0000, GS_PRIV_SMODE1 = 0x0010, GS_PRIV_SMODE2 = 0x0020,
    GS_PRIV_DISPFB1 = 0x0070, GS_PRIV_DISPLAY1 = 0x0080,
    GS_PRIV_DISPFB2 = 0x0090, GS_PRIV_DISPLAY2 = 0x00A0,
    GS_PRIV_BGCOLOR = 0x00E0, GS_PRIV_CSR = 0x1000, GS_PRIV_IMR = 0x1010,
    GS_PRIV_SIGLBLID = 0x1080,
};

#define GS_FIELD(v, lo, bits) ((uint32_t)(((uint64_t)(v) >> (lo)) & ((1ull << (bits)) - 1)))

/* ---- local memory (gs_mem.c) ---- */

/* 16-byte alignment so word/halfword views are fine */
extern uint8_t gs_vram[GS_VRAM_SIZE];
#define GS_VRAM32 ((uint32_t *)gs_vram)
#define GS_VRAM16 ((uint16_t *)gs_vram)

/* Offsets within one page, [y][x], for every layout: the block number
   times the block's size plus the position in the block's columns, in
   the format's own units (words, halfwords, bytes, nibbles). The Z
   formats are the same with the block number xor 0x18. */
extern uint16_t gs_pg32[2][32][64];   /* [0] CT32/24, T8H/T4Hx; [1] Z32/24 */
extern uint16_t gs_pg16[4][64][64];   /* CT16, CT16S, Z16, Z16S */
extern uint16_t gs_pg8[64][128];
extern uint16_t gs_pg4[128][128];

/* Storage layouts */
enum {
    GS_L32, GS_L32Z, GS_L16, GS_L16S, GS_L16Z, GS_L16SZ, GS_L8, GS_L4
};
/* How a pixel is kept: which bits of which unit */
enum {
    GS_K32, GS_K24, GS_K16, GS_K8, GS_K4, GS_K8H, GS_K4HL, GS_K4HH
};

typedef struct GsPsmInfo {
    uint8_t layout;
    uint8_t kind;
    uint8_t valid;
} GsPsmInfo;
extern GsPsmInfo gs_psm[64];

/* Word index (of 1M) of a 32-bit-layout pixel */
static inline uint32_t gs_addr32(int z, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    x &= 2047;
    y &= 2047;
    return (((bp + ((y >> 5) * bw + (x >> 6)) * 32) << 6) + gs_pg32[z][y & 31][x & 63]) & 0xFFFFF;
}
/* Halfword index (of 2M); t is GS_L16 - GS_L16 .. GS_L16SZ - GS_L16 */
static inline uint32_t gs_addr16(int t, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    x &= 2047;
    y &= 2047;
    return (((bp + ((y >> 6) * bw + (x >> 6)) * 32) << 7) + gs_pg16[t][y & 63][x & 63]) & 0x1FFFFF;
}
static inline uint32_t gs_bw8(uint32_t bw) { return bw > 1 ? bw >> 1 : 1; }
/* Byte index (of 4M) */
static inline uint32_t gs_addr8(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    x &= 2047;
    y &= 2047;
    return (((bp + ((y >> 6) * gs_bw8(bw) + (x >> 7)) * 32) << 8) + gs_pg8[y & 63][x & 127]) & 0x3FFFFF;
}
/* Nibble index (of 8M) */
static inline uint32_t gs_addr4(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    x &= 2047;
    y &= 2047;
    return (((bp + ((y >> 7) * gs_bw8(bw) + (x >> 7)) * 32) << 9) + gs_pg4[y & 127][x & 127]) & 0x7FFFFF;
}

void gs_mem_init(void);

/* A pixel's raw value in any PSM (CT32: the word; CT24: the low 24 bits;
   CT16: the halfword; T8/T8H: the index; T4x: the index) */
uint32_t gs_read_pixel(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y);
void gs_write_pixel(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t v);

/* ---- registers and state (gs_regs.c) ---- */

typedef struct GsVertex {
    int32_t x, y; /* 12.4, window coordinates (XYOFFSET subtracted) */
    uint32_t z;
    uint32_t rgba; /* 0xAABBGGRR */
    float s, t, q;
    int32_t u, v; /* 10.4 texels */
    uint32_t f;
} GsVertex;

typedef struct GsContext {
    uint64_t tex0, clamp, tex1, xyoffset, miptbp1, miptbp2;
    uint64_t scissor, alpha, test, fba, frame, zbuf;
} GsContext;

typedef struct GsGifPath {
    uint32_t nloop;  /* loops left in the current tag; 0: next qword is a tag */
    uint32_t nreg, curreg, flg;
    uint64_t regs;
    int ws3d;   /* widescreen: the packet is 3D (gs_widescreen.c) */
    int wsfill; /* widescreen: and is a plane facing the camera spanning the 4:3 screen */
} GsGifPath;

typedef struct GsTransfer {
    int active;
    uint32_t bp, bw, psm;
    uint32_t sx, x, y, w, h, left; /* left: pixels still to go */
    uint8_t pend[8];
    uint32_t npend;
} GsTransfer;

typedef struct GsState {
    uint64_t prim, prmode, prmodecont, rgbaq, st, uv, fog;
    uint64_t texclut, scanmsk, texa, fogcol, dimx, dthe, colclamp, pabe;
    uint64_t bitbltbuf, trxpos, trxreg, trxdir;
    GsContext ctx[2];
    float q; /* the GIF's internal Q (PACKED ST), for PACKED RGBAQ */
    GsVertex vq[3];
    int nv;
    uint16_t clut[512];
    uint32_t cbp0, cbp1;
    GsTransfer up;   /* host to local */
    GsTransfer down; /* local to host */
    GsGifPath path[4];
    int dirty; /* registers the draw setup depends on changed */
    uint8_t *priv;
} GsState;

/* The one GS (a short name inside port/src/gs/, a prefixed one outside) */
#define gs grx_gs_state
extern GsState gs;

/* The privileged registers' memory (default: 0x12000000) */
void grx_gs_set_priv(void *base);
static inline uint64_t gs_priv_read(uint32_t off) {
    uint64_t v;
    memcpy(&v, gs.priv + off, 8);
    return v;
}
static inline void gs_priv_write(uint32_t off, uint64_t v) {
    memcpy(gs.priv + off, &v, 8);
}

void gs_write_reg(uint32_t addr, uint64_t data);

/* ---- transfers and CLUT (gs_mem.c) ---- */
void gs_trx_start(void);           /* TRXDIR written */
void gs_trx_data(const uint8_t *p, uint32_t n); /* HWREG / IMAGE bytes */
void gs_clut_load(uint64_t tex0);  /* TEX0/TEX2 with CLD */

/* ---- drawing: the queue and its worker threads (gs_queue.c, gs_draw.c,
   gs_draw.h) ---- */
void gs_queue_init(void);
/* A primitive kicked with the registers as they are */
void gs_draw_prim(const GsVertex *v, uint32_t type);
/* Once a field (grx_gs_vsync): keeps the queue within a field of the game */
void gs_queue_vsync(void);
/* Finish everything queued (before anything reads or writes local memory
   outside the queue: tests, read-back, local-to-local copies) */
void gs_flush(void);
/* About to read these pages outside the queue: finishes the queued work
   that writes them */
struct GsPages;
void gs_mem_reading(const struct GsPages *p);

/* ---- another renderer behind this front end ----
   The GIF, the registers, the transfers into and out of local memory and
   the CLUT stay here (gs_regs.c, gs_mem.c); a backend (port/src/gs_gl/)
   takes the primitives instead of the software queue, and is told about
   every access to local memory from outside the draws, so it can keep its
   own copies (render targets on the GPU) and gs_vram coherent. NULL (the
   default): the software GS. */
struct GsPages;
typedef struct GsBackend {
    /* A primitive, with the registers as they are (gs.dirty set when they
       changed since the last one) */
    void (*prim)(const GsVertex *v, uint32_t type);
    /* About to write this rectangle of local memory from the CPU side
       (a host-to-local transfer starts, a local-to-local copy's
       destination): gs_vram must hold everything outside it afterwards */
    void (*writing)(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    /* gs_vram's pages were just written (transfer data, a copy) */
    void (*wrote)(const struct GsPages *p);
    /* About to read these pages of gs_vram on the CPU (CLUT loads,
       read-back, a local-to-local copy's source) */
    void (*reading)(const struct GsPages *p);
    /* All of gs_vram changed (a recording's snapshot loaded) */
    void (*loaded)(void);
    void (*vsync)(void);
    /* grx_gs_display for this renderer (pixels in host memory) */
    int (*display)(const uint32_t **pixels, int *width, int *height, int *pitch);
} GsBackend;
extern const GsBackend *gs_backend;

/* ---- widescreen (gs_widescreen.c, port/docs/widescreen.md) ---- */
extern int gs_ws_on;        /* the aspect may be wider than 4:3: track pages */
extern float gs_ws_k;       /* the horizontal squeeze, 1 at 4:3 (nothing changes) */
extern float gs_ws_aspect;  /* the display aspect frames are drawn for */
extern int gs_ws_3d;        /* the GIF packet being read is 3D */
extern int gs_ws_fill;      /* ... and is a screen-filling plane (gs_ws_tag_fills) */
extern int gs_ws_squeezed;  /* the primitive being drawn was squeezed */
void gs_ws_set(int on, float aspect);
void gs_ws_forget(void);
/* The primitive to draw: v, or tmp filled with v squeezed */
const GsVertex *gs_ws_prim(const GsVertex *v, uint32_t type, GsVertex *tmp);
int gs_ws_tag_fills(uint64_t tag_lo, uint64_t regs, const uint8_t *data, uint32_t qwc);
void gs_ws_uploaded(const struct GsPages *p);
/* A read-back of n bytes to dst, and an upload's data: frames captured to
   main memory and uploaded again stay full width (gs_widescreen.c) */
void gs_ws_read_back(const void *dst, uint32_t bytes, const struct GsPages *p);
void gs_ws_upload_data(const void *data, const struct GsPages *p);
uint64_t gs_ws_scissor_wide(uint64_t sc);
static inline uint64_t gs_ws_scissor(uint64_t sc) {
    return gs_ws_k == 1.0f ? sc : gs_ws_scissor_wide(sc);
}

/* ---- recording (gs_record.c, for port/tests/gs_replay.c) ---- */
#define GS_REC_PRIV 0x100        /* privileged registers kept per vsync */
#define GS_REC_PRIV_STATE 0x1100 /* ... and in a snapshot (with CSR) */
#define GS_REC_STATE_MAX 0x2000
extern int gs_rec_active; /* recording now */
void gs_rec_gif(const void *data, uint32_t qwc, int path);
void gs_rec_event(int what);
void gs_rec_read_image(const void *data, uint32_t qwc);
void gs_rec_vsync(void);
uint32_t gs_rec_save_state(uint8_t *buf);
uint32_t gs_rec_load_state(const uint8_t *buf);
void gs_rec_load(const uint8_t *state, const uint8_t *vram);

#endif
