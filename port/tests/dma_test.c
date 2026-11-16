/*
 * Tests for the port's DMA controller and VIF (port/src/dma.c, vif.c).
 * A standalone program, with small fakes of the GS (grx_gs_gif,
 * grx_gs_reset), the kernel's DMAC interrupt and VU1 that record what
 * they get:
 *
 *   clang -target i686-w64-mingw32 -std=gnu11 -Wall -Iport/include \
 *       port/tests/dma_test.c port/src/dma.c port/src/vif.c port/src/hwmem.c -o dma_test.exe
 *
 * The hardware windows (registers, scratchpad, VU memory) come from
 * grx_hwmem_init(), as in the port. Exit status: the failure count.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_dma.h"
#include "grx_vif.h"
#include "grx_vu1.h"
#include "grx_gs.h"
#include "grx_hwmem.h"

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

static int quiet_log;
static int log_count;

void grx_log(const char *fmt, ...) {
    va_list ap;
    log_count++;
    if (quiet_log) {
        return;
    }
    va_start(ap, fmt);
    printf("  log: ");
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    exit(100);
}

/* ---------------------------------------------------------------------- */
/* Fakes */

#define MAXQ 4096
static uint32_t gif_q[MAXQ][4];
static int gif_path[MAXQ];
static int gif_n, gif_calls;

void grx_gs_gif(const void *data, uint32_t qwc, int path) {
    const uint32_t *w = data;
    uint32_t i;
    gif_calls++;
    for (i = 0; i < qwc && gif_n < MAXQ; i++) {
        memcpy(gif_q[gif_n], w + i * 4, 16);
        gif_path[gif_n++] = path;
    }
}

static int gs_resets;

void grx_gs_reset(void) {
    gs_resets++;
    GRX_HW64(GRX_GS_CSR) = (GRX_HW64(GRX_GS_CSR) & (1u << 13)) | 0x551B4000u;
}

static int raised[16];
static int raise_order[64], raise_n;
static void (*on_raise)(int ch);

void grx_kernel_raise_dmac(int ch) {
    raised[ch]++;
    if (raise_n < 64) {
        raise_order[raise_n++] = ch;
    }
    if (on_raise) {
        on_raise(ch);
    }
}

#define MAXRUN 64
static uint32_t vu_addr[MAXRUN], vu_top[MAXRUN], vu_itop[MAXRUN];
static uint32_t vu_seen[MAXRUN][4]; /* VU1 data at vu_watch when it ran */
static uint32_t vu_watch;
static int vu_n;

void grx_vu1_run(uint32_t addr) {
    if (vu_n < MAXRUN) {
        vu_addr[vu_n] = addr;
        vu_top[vu_n] = grx_vif1_top();
        vu_itop[vu_n] = grx_vif1_itop();
        memcpy(vu_seen[vu_n], grx_vu1_data + vu_watch * 16, 16);
        vu_n++;
    }
}

static void reset_fakes(void) {
    gif_n = gif_calls = 0;
    memset(raised, 0, sizeof raised);
    raise_n = 0;
    vu_n = 0;
    on_raise = NULL;
    log_count = 0;
}

/* ---------------------------------------------------------------------- */
/* Helpers */

#define REG(ch, off) GRX_HW32(grx_dma_chan_base(ch) + (off))
#define A(p) ((uint32_t)(uintptr_t)(p))
#define SPRP(off) ((uint32_t *)(uintptr_t)(0x70000000u + (off)))

enum { REFE, CNT, NEXT, REF, REFS, CALL, RET, END };

static void tag(uint32_t *q, uint32_t qwc, uint32_t id, int irq, uint32_t addr, uint32_t w2, uint32_t w3) {
    q[0] = qwc | id << 28 | (uint32_t)irq << 31;
    q[1] = addr;
    q[2] = w2;
    q[3] = w3;
}

static void mark(uint32_t *q, uint32_t m) {
    q[0] = m;
    q[1] = m + 1;
    q[2] = m + 2;
    q[3] = m + 3;
}

static int gif_marks(const uint32_t *want, int n, int path) {
    int i;
    if (gif_n != n) {
        printf("  got %d quadwords, want %d:", gif_n, n);
        for (i = 0; i < gif_n; i++) {
            printf(" %X", gif_q[i][0]);
        }
        printf("\n");
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (gif_q[i][0] != want[i] || gif_q[i][3] != want[i] + 3 || gif_path[i] != path) {
            printf("  quadword %d: %X (path %d), want %X (path %d)\n", i, gif_q[i][0], gif_path[i], want[i], path);
            return 0;
        }
    }
    return 1;
}

static uint32_t mem[0x2000][4] __attribute__((aligned(16)));

static uint32_t *vu1(uint32_t qw) {
    return (uint32_t *)(grx_vu1_data + qw * 16);
}

/* ---------------------------------------------------------------------- */
/* Tests */

static void test_normal_gif(void) {
    uint32_t (*b)[4] = mem + 0x10;
    uint32_t want[3] = {0x100, 0x200, 0x300};
    reset_fakes();
    mark(b[0], 0x100);
    mark(b[1], 0x200);
    mark(b[2], 0x300);
    REG(2, GRX_DMA_MADR) = A(b);
    REG(2, GRX_DMA_QWC) = 3;
    GRX_HW32(GRX_D_STAT) = 0;
    grx_dma_write_chcr(GRX_DMA_GIF, 0x101);
    CHECK(gif_marks(want, 3, GRX_GIF_PATH3), "normal GIF data");
    CHECK(REG(2, GRX_DMA_CHCR) == 0x001, "CHCR 0x%X: STR not cleared", REG(2, GRX_DMA_CHCR));
    CHECK(REG(2, GRX_DMA_QWC) == 0, "QWC not 0");
    CHECK(REG(2, GRX_DMA_MADR) == A(b + 3), "MADR 0x%X, want 0x%X", REG(2, GRX_DMA_MADR), A(b + 3));
    CHECK(GRX_HW32(GRX_D_STAT) == 4, "D_STAT 0x%X", GRX_HW32(GRX_D_STAT));
    CHECK(raised[2] == 1 && raise_n == 1, "GIF interrupt raised %d times", raised[2]);
    CHECK(gif_calls == 1, "contiguous data in one call (%d)", gif_calls);

    /* A start with STR clear does nothing */
    reset_fakes();
    grx_dma_write_chcr(GRX_DMA_GIF, 0x001);
    CHECK(gif_n == 0 && raise_n == 0, "a store without STR started something");
}

/* refe-less chain with every tag: cnt, next, ref, call (into a cnt/ret
   subroutine), refs, end */
static void test_chain_gif(void) {
    uint32_t (*t)[4] = mem + 0x100;
    uint32_t (*r)[4] = mem + 0x200;
    uint32_t (*sub)[4] = mem + 0x300;
    uint32_t want[] = {0xA0, 0xB0, 0x10, 0x20, 0xC0, 0xE0, 0xF0, 0x30, 0xD0};
    uint32_t chcr;

    reset_fakes();
    memset(t, 0, 16 * 16);
    tag(t[0], 1, CNT, 0, 0, 0, 0);
    mark(t[1], 0xA0);
    tag(t[2], 1, NEXT, 0, A(t + 5), 0, 0);
    mark(t[3], 0xB0);
    mark(t[4], 0xBAD); /* skipped by next */
    tag(t[5], 2, REF, 0, A(r), 0, 0);
    tag(t[6], 1, CALL, 0, A(sub), 0, 0);
    mark(t[7], 0xC0);
    tag(t[8], 1, REFS, 0, A(r + 2), 0, 0);
    tag(t[9], 1, END, 0, 0, 0, 0);
    mark(t[10], 0xD0);
    mark(r[0], 0x10);
    mark(r[1], 0x20);
    mark(r[2], 0x30);
    tag(sub[0], 1, CNT, 0, 0, 0, 0);
    mark(sub[1], 0xE0);
    tag(sub[2], 1, RET, 0, 0, 0, 0);
    mark(sub[3], 0xF0);

    REG(2, GRX_DMA_TADR) = A(t);
    REG(2, GRX_DMA_QWC) = 0;
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    CHECK(gif_marks(want, 9, GRX_GIF_PATH3), "chain order");
    chcr = REG(2, GRX_DMA_CHCR);
    CHECK((chcr & 0x1FF) == 0x005, "CHCR 0x%X: STR or ASP not cleared", chcr);
    CHECK(chcr >> 16 == (t[9][0] >> 16), "CHCR.TAG 0x%X, want the end tag's 0x%X", chcr >> 16, t[9][0] >> 16);
    CHECK(REG(2, GRX_DMA_TADR) == A(t + 9), "TADR 0x%X, want the end tag 0x%X", REG(2, GRX_DMA_TADR), A(t + 9));
    CHECK(REG(2, GRX_DMA_MADR) == A(t + 11), "MADR 0x%X, want 0x%X", REG(2, GRX_DMA_MADR), A(t + 11));
    CHECK(REG(2, GRX_DMA_ASR0) == A(t + 8), "ASR0 0x%X, want 0x%X", REG(2, GRX_DMA_ASR0), A(t + 8));
    CHECK(raised[2] == 1, "raised %d", raised[2]);

    /* refe ends at once; TADR moves past it */
    reset_fakes();
    tag(t[0], 2, REFE, 0, A(r), 0, 0);
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    CHECK(gif_marks(want + 2, 2, GRX_GIF_PATH3), "refe");
    CHECK(REG(2, GRX_DMA_TADR) == A(t + 1) && REG(2, GRX_DMA_MADR) == A(r + 2), "refe TADR 0x%X MADR 0x%X",
          REG(2, GRX_DMA_TADR), REG(2, GRX_DMA_MADR));

    /* Nested calls (ASP 2), and ret at depth 0 ends */
    reset_fakes();
    tag(t[0], 0, CALL, 0, A(sub), 0, 0);
    tag(t[1], 1, RET, 0, 0, 0, 0); /* ASP 0: ends */
    mark(t[2], 0x77);
    tag(sub[0], 0, CALL, 0, A(sub + 4), 0, 0);
    tag(sub[1], 0, RET, 0, 0, 0, 0);
    tag(sub[4], 1, RET, 0, 0, 0, 0);
    mark(sub[5], 0x66);
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    {
        uint32_t w2[] = {0x66, 0x77};
        CHECK(gif_marks(w2, 2, GRX_GIF_PATH3), "nested calls");
    }
    CHECK((REG(2, GRX_DMA_CHCR) & 0x130) == 0, "ASP 0x%X at the end", (REG(2, GRX_DMA_CHCR) >> 4) & 3);

    /* Interrupt bit with TIE: the transfer stops after that tag */
    reset_fakes();
    tag(t[0], 1, CNT, 1, 0, 0, 0);
    mark(t[1], 0xA0);
    tag(t[2], 1, END, 0, 0, 0, 0);
    mark(t[3], 0xD0);
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x185);
    CHECK(gif_marks(want, 1, GRX_GIF_PATH3), "IRQ + TIE stops after the tag");
    CHECK(REG(2, GRX_DMA_TADR) == A(t + 2), "TADR 0x%X after the IRQ tag", REG(2, GRX_DMA_TADR));
    CHECK(!(REG(2, GRX_DMA_CHCR) & 0x100) && raised[2] == 1, "stopped and raised");
    /* ...and without TIE it goes on */
    reset_fakes();
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    {
        uint32_t w2[] = {0xA0, 0xD0};
        CHECK(gif_marks(w2, 2, GRX_GIF_PATH3), "IRQ without TIE");
    }

    /* A start with QWC left finishes those first (the TAG field's id says
       whether it goes on: cnt here) */
    reset_fakes();
    REG(2, GRX_DMA_MADR) = A(r);
    REG(2, GRX_DMA_QWC) = 1;
    REG(2, GRX_DMA_TADR) = A(t + 2);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x10000105);
    {
        uint32_t w2[] = {0x10, 0xD0};
        CHECK(gif_marks(w2, 2, GRX_GIF_PATH3), "resume with QWC");
    }

    /* A loop is cut off, a bad address doesn't hang */
    reset_fakes();
    quiet_log = 1;
    tag(t[0], 0, NEXT, 0, A(t), 0, 0);
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    CHECK(log_count > 0 && raised[2] == 1 && !(REG(2, GRX_DMA_CHCR) & 0x100), "an endless chain");
    reset_fakes();
    tag(t[0], 4, REF, 0, 0x100, 0, 0);
    tag(t[1], 0, END, 0, 0, 0, 0);
    REG(2, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_GIF, 0x105);
    CHECK(log_count > 0 && gif_n == 0 && raised[2] == 1 && !(REG(2, GRX_DMA_CHCR) & 0x100), "a bad address");
    quiet_log = 0;
}

/* Tags and data in the scratchpad: TADR bit 31, the tag's SPR bit, and
   raw 0x7000xxxx addresses; a uncached-alias address of main memory */
static void test_chain_spr(void) {
    uint32_t (*r)[4] = mem + 0x400;
    uint32_t want[] = {0x11, 0x22, 0x33, 0x44};

    reset_fakes();
    tag(SPRP(0x100), 1, CNT, 0, 0, 0, 0);
    mark(SPRP(0x110), 0x11);
    tag(SPRP(0x120), 1, REF, 0, 0x80000000u | 0x200, 0, 0); /* SPR bit */
    mark(SPRP(0x200), 0x22);
    tag(SPRP(0x130), 1, REF, 0, 0x70000210u, 0, 0); /* a raw scratchpad address */
    mark(SPRP(0x210), 0x33);
    tag(SPRP(0x140), 1, REFE, 0, A(r) | 0x20000000u, 0, 0); /* uncached alias */
    mark(r[0], 0x44);
    REG(2, GRX_DMA_TADR) = 0x80000100u;
    grx_dma_kick((volatile void *)(uintptr_t)GRX_DMA_D2); /* as sceDmaSend does, after storing CHCR */
    CHECK(gif_marks(want, 0, GRX_GIF_PATH3), "a kick with STR clear started something");
    REG(2, GRX_DMA_CHCR) = 0x105;
    grx_dma_kick((volatile void *)(uintptr_t)GRX_DMA_D2);
    CHECK(gif_marks(want, 4, GRX_GIF_PATH3), "scratchpad chain");
    CHECK(REG(2, GRX_DMA_TADR) == 0x80000150u, "TADR 0x%X keeps the SPR bit", REG(2, GRX_DMA_TADR));

    /* A normal transfer that runs off the scratchpad's end wraps */
    reset_fakes();
    mark(SPRP(0x3FF0), 0x55);
    mark(SPRP(0x0000), 0x66);
    REG(2, GRX_DMA_MADR) = 0x80003FF0u;
    REG(2, GRX_DMA_QWC) = 2;
    grx_dma_write_chcr(GRX_DMA_GIF, 0x101);
    {
        uint32_t w2[] = {0x55, 0x66};
        CHECK(gif_marks(w2, 2, GRX_GIF_PATH3), "scratchpad wrap");
    }
    CHECK(REG(2, GRX_DMA_MADR) == 0x80000010u, "MADR 0x%X after the wrap", REG(2, GRX_DMA_MADR));
}

static void vif1_send(const uint32_t *w, uint32_t qwc) {
    REG(1, GRX_DMA_MADR) = A(w);
    REG(1, GRX_DMA_QWC) = qwc;
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x101);
}

static void test_vif_unpack(void) {
    uint32_t *p = mem[0x500];
    uint32_t *v;
    int i, n = 0;
    float f;

    reset_fakes();
    memset(p, 0, 0x400);
    memset(grx_vu1_data, 0xEE, GRX_VU1_DATA_SIZE);
    /* STCYCL 4,4; UNPACK V4-32 2 to 0x10; data */
    p[n++] = 0x01000404;
    p[n++] = 0x6C020010;
    for (i = 0; i < 8; i++) {
        p[n++] = 0x1000 + i;
    }
    /* V3-16 signed, 2 to 0x20 (3 words of data: 6 halfwords) */
    p[n++] = 0x69020020;
    p[n++] = 0x0002FFFF; /* -1, 2 */
    p[n++] = 0x8000FFFD; /* -3, -32768 */
    p[n++] = 0x00070006;
    /* V4-8 unsigned to 0x30 */
    p[n++] = 0x6E014030;
    p[n++] = 0x80FF0201;
    /* V4-8 signed to 0x31 */
    p[n++] = 0x6E010031;
    p[n++] = 0x80FF0201;
    /* V4-5 to 0x32: r 31, g 1, b 16, a 1 */
    p[n++] = 0x6F010032;
    p[n++] = 0x0000C03F; /* 1 10000 00001 11111 */
    /* S-32 to 0x33; V2-16 unsigned to 0x34 */
    p[n++] = 0x60010033;
    p[n++] = 0xCAFEF00D;
    p[n++] = 0x65014034;
    p[n++] = 0x00050004;
    vif1_send(p, (n + 3) / 4);
    v = vu1(0x10);
    CHECK(v[0] == 0x1000 && v[3] == 0x1003 && v[4] == 0x1004 && v[7] == 0x1007, "V4-32");
    v = vu1(0x20);
    CHECK(v[0] == 0xFFFFFFFF && v[1] == 2 && v[2] == 0xFFFFFFFD, "V3-16 x y z: %X %X %X", v[0], v[1], v[2]);
    CHECK(v[3] == 0xFFFF8000, "V3-16 w: the next halfword (%X)", v[3]);
    CHECK(v[4] == 0xFFFF8000 && v[5] == 6 && v[6] == 7, "V3-16 second: %X %X %X", v[4], v[5], v[6]);
    v = vu1(0x30);
    CHECK(v[0] == 1 && v[1] == 2 && v[2] == 0xFF && v[3] == 0x80, "V4-8 usn");
    v = vu1(0x31);
    CHECK(v[0] == 1 && v[1] == 2 && v[2] == 0xFFFFFFFF && v[3] == 0xFFFFFF80, "V4-8 signed");
    v = vu1(0x32);
    CHECK(v[0] == 0xF8 && v[1] == 0x08 && v[2] == 0x80 && v[3] == 0x80, "V4-5: %X %X %X %X", v[0], v[1], v[2],
          v[3]);
    v = vu1(0x33);
    CHECK(v[0] == 0xCAFEF00D && v[3] == 0xCAFEF00D, "S-32");
    v = vu1(0x34);
    CHECK(v[0] == 4 && v[1] == 5 && v[2] == 4 && v[3] == 5, "V2-16: x y x y");
    CHECK(vu1(0x12)[0] == 0xEEEEEEEE, "UNPACK wrote too far");
    CHECK(raised[1] == 1 && REG(1, GRX_DMA_QWC) == 0, "VIF1 DMA completed");

    /* Masks, rows, columns; STMOD offset and difference */
    reset_fakes();
    memset(p, 0, 0x400);
    n = 0;
    p[n++] = 0x30000000; /* STROW */
    p[n++] = 10;
    p[n++] = 20;
    p[n++] = 30;
    f = 1.0f;
    memcpy(&p[n++], &f, 4);
    p[n++] = 0x31000000; /* STCOL */
    p[n++] = 100;
    p[n++] = 200;
    p[n++] = 300;
    p[n++] = 400;
    p[n++] = 0x20000000; /* STMASK: row 0: x data, y row, z col, w protect; rows 1-3: data, z w row */
    p[n++] = 0xE4 | 0x50 << 8 | 0x50 << 16 | 0x50 << 24;
    p[n++] = 0x01000404;
    p[n++] = 0x75024040; /* masked V2-16 usn, 2 to 0x40 */
    p[n++] = 0x00020001;
    p[n++] = 0x00040003;
    p[n++] = 0x05000001; /* STMOD offset */
    p[n++] = 0x6C010042; /* V4-32 1 to 0x42 */
    p[n++] = 1;
    p[n++] = 2;
    p[n++] = 3;
    p[n++] = 4;
    p[n++] = 0x05000002; /* STMOD difference */
    p[n++] = 0x6C010043;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 0x6C010044;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 1;
    p[n++] = 0x05000000;
    memset(vu1(0x40), 0x5A, 32);
    vif1_send(p, (n + 3) / 4);
    v = vu1(0x40);
    CHECK(v[0] == 1 && v[1] == 20 && v[2] == 100 && v[3] == 0x5A5A5A5A, "mask row 0: %u %u %u %X", v[0], v[1],
          v[2], v[3]);
    CHECK(v[4] == 3 && v[5] == 4 && v[6] == 30 && v[7] == 0x3F800000, "mask row 1: %u %u %u %X", v[4], v[5],
          v[6], v[7]);
    v = vu1(0x42);
    CHECK(v[0] == 11 && v[1] == 22 && v[2] == 33, "STMOD offset: %u %u %u", v[0], v[1], v[2]);
    v = vu1(0x43);
    CHECK(v[0] == 11 && v[4] == 12 && v[5] == 22, "STMOD difference: %u %u %u", v[0], v[4], v[5]);
    CHECK(GRX_HW32(0x10003D00) == 12, "VIF1_R0 0x%X in the page after difference mode", GRX_HW32(0x10003D00));

    /* Skip mode (CL 4, WL 2) and fill mode (CL 1, WL 3) */
    reset_fakes();
    memset(p, 0, 0x400);
    n = 0;
    p[n++] = 0x01000204; /* WL 2, CL 4 */
    p[n++] = 0x60040050; /* S-32 4 to 0x50 */
    p[n++] = 1;
    p[n++] = 2;
    p[n++] = 3;
    p[n++] = 4;
    p[n++] = 0x30000000;
    p[n++] = 9;
    p[n++] = 9;
    p[n++] = 9;
    p[n++] = 9;
    p[n++] = 0x01000301; /* WL 3, CL 1 */
    p[n++] = 0x60060060; /* S-32 6 writes to 0x60: 2 inputs */
    p[n++] = 7;
    p[n++] = 8;
    p[n++] = 0x01000404;
    memset(vu1(0x50), 0, 0x20 * 16);
    vif1_send(p, (n + 3) / 4);
    CHECK(vu1(0x50)[0] == 1 && vu1(0x51)[0] == 2 && vu1(0x52)[0] == 0 && vu1(0x53)[0] == 0 &&
              vu1(0x54)[0] == 3 && vu1(0x55)[0] == 4 && vu1(0x56)[0] == 0,
          "skip mode: %u %u %u %u %u %u", vu1(0x50)[0], vu1(0x51)[0], vu1(0x52)[0], vu1(0x53)[0], vu1(0x54)[0],
          vu1(0x55)[0]);
    CHECK(vu1(0x60)[0] == 7 && vu1(0x61)[0] == 9 && vu1(0x62)[0] == 9 && vu1(0x63)[0] == 8 &&
              vu1(0x64)[1] == 9 && vu1(0x66)[0] == 0,
          "fill mode: %u %u %u %u %u", vu1(0x60)[0], vu1(0x61)[0], vu1(0x62)[0], vu1(0x63)[0], vu1(0x64)[0]);
}

/* MPG, MSCAL deferred to the next wait, TOPS/TOP double buffering, ITOP,
   MSCNT, DIRECT, MARK */
static void test_vif_vu(void) {
    uint32_t *p = mem[0x600];
    int n = 0, i;

    reset_fakes();
    memset(p, 0, 0x400);
    vu_watch = 0x3F0;
    p[n++] = 0x03000100; /* BASE 0x100 */
    p[n++] = 0x02000080; /* OFFSET 0x80 */
    p[n++] = 0x04000123; /* ITOP 0x123 */
    p[n++] = 0x4A020004; /* MPG 2 pairs to pair 4 */
    p[n++] = 0x11111111;
    p[n++] = 0x22222222;
    p[n++] = 0x33333333;
    p[n++] = 0x44444444;
    p[n++] = 0x14000002; /* MSCAL 2: TOP 0x100, then TOPS 0x180 */
    p[n++] = 0x6C0103F0; /* data for the program, after the MSCAL */
    p[n++] = 0xAB;
    p[n++] = 0;
    p[n++] = 0;
    p[n++] = 0;
    p[n++] = 0x6C018000; /* UNPACK with FLG: to TOPS + 0 = 0x180 */
    p[n++] = 0xCD;
    p[n++] = 0;
    p[n++] = 0;
    p[n++] = 0;
    p[n++] = 0x10000000; /* FLUSHE: the program runs here */
    p[n++] = 0x17000000; /* MSCNT: TOP 0x180, TOPS 0x100 */
    p[n++] = 0x07001234; /* MARK */
    p[n++] = 0x00000000;
    p[n++] = 0x50000002; /* DIRECT 2 */
    for (i = 0; i < 8; i++) {
        p[n++] = 0xD00 + i;
    }
    p[n++] = 0x00000000; /* NOP x3, then MSCAL left pending at the end */
    p[n++] = 0x00000000;
    p[n++] = 0x00000000;
    p[n++] = 0x15000010; /* MSCALF 0x10 */
    vif1_send(p, n / 4);
    CHECK(memcmp(grx_vu1_micro + 32, &p[4], 16) == 0, "MPG into micro memory at pair 4");
    CHECK(vu_n == 3, "VU1 ran %d times, want 3", vu_n);
    if (vu_n >= 1) {
        CHECK(vu_addr[0] == 2 && vu_top[0] == 0x100 && vu_itop[0] == 0x123, "MSCAL: addr %X TOP %X ITOP %X",
              vu_addr[0], vu_top[0], vu_itop[0]);
        CHECK(vu_seen[0][0] == 0xAB, "the MSCAL'd program ran before the data after it came (%X)", vu_seen[0][0]);
    }
    if (vu_n >= 2) {
        CHECK(vu_addr[1] == GRX_VU1_CONTINUE && vu_top[1] == 0x180, "MSCNT: addr %X TOP %X", vu_addr[1],
              vu_top[1]);
    }
    if (vu_n >= 3) {
        CHECK(vu_addr[2] == 0x10 && vu_top[2] == 0x100, "MSCALF run at the end of the transfer: %X TOP %X",
              vu_addr[2], vu_top[2]);
    }
    CHECK(vu1(0x180)[0] == 0xCD, "UNPACK FLG went to TOPS");
    CHECK(GRX_HW32(0x10003C30) == 0x1234, "VIF1_MARK 0x%X", GRX_HW32(0x10003C30));
    CHECK(gif_n == 2 && gif_path[0] == GRX_GIF_PATH2 && gif_q[0][0] == 0xD00 && gif_q[1][3] == 0xD07,
          "DIRECT to PATH2 (%d)", gif_n);
    CHECK(GRX_HW32(0x10003CE0) == 0x100 && GRX_HW32(0x10003CD0) == 0x123, "TOP/ITOP in the page");
}

/* TTE (VIF codes in the tags), commands split over DMA tags and over
   transfers, a DIRECT quadword split by a tag */
static void test_vif_split(void) {
    uint32_t (*t)[4] = mem + 0x700;
    uint32_t (*d)[4] = mem + 0x780;

    reset_fakes();
    memset(t, 0, 16 * 16);
    memset(vu1(0x200), 0, 0x40 * 16);
    /* TTE: the tag's upper words go to VIF1 ahead of the data */
    tag(t[0], 1, CNT, 0, 0, 0x01000404, 0x6C010200); /* STCYCL 4,4; UNPACK V4-32 1 to 0x200 */
    mark(t[1], 0x900);
    tag(t[2], 1, END, 0, 0, 0, 0x50000001); /* NOP; DIRECT 1 */
    mark(t[3], 0xB00);
    REG(1, GRX_DMA_TADR) = A(t);
    REG(1, GRX_DMA_QWC) = 0;
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x145);
    CHECK(vu1(0x200)[0] == 0x900 && vu1(0x200)[3] == 0x903, "TTE: UNPACK in the tag");
    CHECK(gif_n == 1 && gif_q[0][0] == 0xB00 && gif_path[0] == GRX_GIF_PATH2, "TTE: DIRECT in the tag (%d)", gif_n);

    /* Without TTE: an UNPACK of 3 whose data is a cnt's last quadword and
       two by ref */
    reset_fakes();
    memset(t, 0, 16 * 16);
    tag(t[0], 2, CNT, 0, 0, 0xDEAD, 0xBEEF); /* upper words not sent */
    t[1][0] = 0x01000404;
    t[1][3] = 0x6C030210;
    mark(t[2], 0x910);
    tag(t[3], 2, REF, 0, A(d), 0, 0);
    tag(t[4], 0, END, 0, 0, 0, 0);
    mark(d[0], 0x920);
    mark(d[1], 0x930);
    REG(1, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x105);
    CHECK(vu1(0x210)[0] == 0x910 && vu1(0x211)[0] == 0x920 && vu1(0x212)[3] == 0x933, "UNPACK across tags");
    CHECK(!grx_vif_busy(1), "VIF1 idle after it");

    /* DIRECT 2 whose second quadword is cut by a tag's upper words (TTE) */
    reset_fakes();
    memset(t, 0, 16 * 16);
    tag(t[0], 1, CNT, 0, 0, 0, 0x50000002);
    mark(t[1], 0xA00);
    tag(t[2], 1, END, 0, 0, 0x111, 0x222);
    t[3][0] = 0x333;
    t[3][1] = 0x444; /* then two NOPs */
    REG(1, GRX_DMA_TADR) = A(t);
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x145);
    CHECK(gif_n == 2 && gif_q[0][0] == 0xA00, "DIRECT across tags: %d", gif_n);
    if (gif_n == 2) {
        CHECK(gif_q[1][0] == 0x111 && gif_q[1][1] == 0x222 && gif_q[1][2] == 0x333 && gif_q[1][3] == 0x444,
              "DIRECT's second quadword: %X %X %X %X", gif_q[1][0], gif_q[1][1], gif_q[1][2], gif_q[1][3]);
    }
    CHECK(!grx_vif_busy(1), "VIF1 idle after it");

    /* An UNPACK whose data comes in a later transfer */
    reset_fakes();
    memset(d, 0, 2 * 16);
    d[0][3] = 0x6C010220;
    vif1_send(d[0], 1);
    CHECK(grx_vif_busy(1), "UNPACK waits for its data");
    mark(d[1], 0xC00);
    vif1_send(d[1], 1);
    CHECK(!grx_vif_busy(1) && vu1(0x220)[0] == 0xC00, "UNPACK finished by the next transfer");
}

/* MFIFO: GsMfifoStart's set-up, packets sent through fromSPR, the ring
   wrapping (and not aligned to its size), the stall, then an end tag */
static void test_mfifo(void) {
    uint32_t (*ring)[4] = mem + 0x801; /* 8 quadwords, not 128-aligned */
    uint32_t base = A(ring), k;
    uint32_t *pkt = SPRP(0x1800);

    reset_fakes();
    GRX_HW32(GRX_D_CTRL) = 1 | 2 << 2; /* DMAE, MFD = VIF1 */
    GRX_HW32(GRX_D_RBOR) = base;
    GRX_HW32(GRX_D_RBSR) = 8 * 16 - 16;
    REG(8, GRX_DMA_MADR) = base;
    REG(8, GRX_DMA_CHCR) = 0;
    REG(1, GRX_DMA_TADR) = base;
    REG(1, GRX_DMA_QWC) = 0;
    grx_dma_write_chcr(GRX_DMA_VIF1, 0x105);
    CHECK(REG(1, GRX_DMA_CHCR) & 0x100, "the drain stays started on the empty ring");
    CHECK(raise_n == 0, "nothing raised at the start");

    for (k = 0; k < 5; k++) {
        /* cnt 2: NOP NOP NOP DIRECT 1, then a GIF quadword */
        tag(pkt, 2, CNT, 0, 0, 0, 0);
        pkt[4] = pkt[5] = pkt[6] = 0;
        pkt[7] = 0x50000001;
        mark(pkt + 8, 0xE00 + k * 0x10);
        REG(8, GRX_DMA_SADR) = 0x1800;
        REG(8, GRX_DMA_QWC) = 3;
        /* GsMfifoSend's room check */
        CHECK(REG(8, GRX_DMA_MADR) == REG(1, GRX_DMA_TADR), "ring not empty before send %u", k);
        grx_dma_write_chcr(GRX_DMA_FROM_SPR, 0x100);
        CHECK(gif_n == (int)k + 1 && gif_q[k][0] == 0xE00 + k * 0x10 && gif_path[k] == GRX_GIF_PATH2,
              "packet %u through the ring (%d)", k, gif_n);
        CHECK(REG(8, GRX_DMA_MADR) == base + ((k + 1) * 48) % 128, "fromSPR MADR 0x%X in the ring",
              REG(8, GRX_DMA_MADR));
        CHECK(REG(1, GRX_DMA_TADR) == REG(8, GRX_DMA_MADR), "the drain caught up");
        CHECK((REG(1, GRX_DMA_CHCR) & 0x100) && !(REG(8, GRX_DMA_CHCR) & 0x100), "D1 stalled, D8 done");
    }
    CHECK(raised[8] == 5 && raised[1] == 0, "raised fromSPR %d, VIF1 %d", raised[8], raised[1]);
    quiet_log = 1;
    CHECK(grx_dma_wait(GRX_DMA_VIF1) != 0, "waiting for the stalled drain reports it");
    quiet_log = 0;

    /* GsMfifoFlush's empty ref, then an end tag: VIF1 finishes */
    tag(pkt, 0, REF, 0, 0, 0, 0);
    REG(8, GRX_DMA_SADR) = 0x1800;
    REG(8, GRX_DMA_QWC) = 1;
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, 0x100);
    CHECK(REG(1, GRX_DMA_TADR) == REG(8, GRX_DMA_MADR), "GsMfifoIsBusy's MADR == TADR after the flush");
    tag(pkt, 0, END, 0, 0, 0, 0);
    REG(8, GRX_DMA_SADR) = 0x1800;
    REG(8, GRX_DMA_QWC) = 1;
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, 0x100);
    CHECK(!(REG(1, GRX_DMA_CHCR) & 0x100) && raised[1] == 1, "the end tag ends the drain");
    CHECK(raise_order[raise_n - 2] == 1 && raise_order[raise_n - 1] == 8, "VIF1's interrupt with fromSPR's");
    GRX_HW32(GRX_D_CTRL) = 1;
}

static void restart_gif(int ch) {
    if (ch == GRX_DMA_GIF) {
        on_raise = NULL;
        REG(2, GRX_DMA_QWC) = 1;
        grx_dma_write_chcr(GRX_DMA_GIF, 0x101); /* a handler starting the next transfer */
    }
}

/* The scratchpad channels: normal, interleave, toSPR chain with TTE,
   fromSPR destination chain; a handler that starts another transfer */
static void test_spr(void) {
    uint32_t (*b)[4] = mem + 0x900;
    uint32_t (*t)[4] = mem + 0x980;
    uint32_t (*o)[4] = mem + 0xA00;
    int i;

    reset_fakes();
    for (i = 0; i < 8; i++) {
        mark(b[i], 0x100 * (i + 1));
    }
    /* toSPR normal */
    REG(9, GRX_DMA_MADR) = A(b);
    REG(9, GRX_DMA_SADR) = 0x2000;
    REG(9, GRX_DMA_QWC) = 2;
    grx_dma_write_chcr(GRX_DMA_TO_SPR, 0x100);
    CHECK(SPRP(0x2000)[0] == 0x100 && SPRP(0x2010)[3] == 0x203, "toSPR normal");
    CHECK(REG(9, GRX_DMA_SADR) == 0x2020 && REG(9, GRX_DMA_MADR) == A(b + 2), "toSPR write-back");
    /* toSPR interleave: TQWC 1, SQWC 1 (every other quadword) */
    GRX_HW32(GRX_D_SQWC) = 1 << 16 | 1;
    REG(9, GRX_DMA_MADR) = A(b);
    REG(9, GRX_DMA_SADR) = 0x2100;
    REG(9, GRX_DMA_QWC) = 3;
    grx_dma_write_chcr(GRX_DMA_TO_SPR, 0x108);
    CHECK(SPRP(0x2100)[0] == 0x100 && SPRP(0x2110)[0] == 0x300 && SPRP(0x2120)[0] == 0x500, "toSPR interleave");
    /* fromSPR normal and interleave (skipping on the memory side) */
    memset(o, 0, 8 * 16);
    REG(8, GRX_DMA_MADR) = A(o);
    REG(8, GRX_DMA_SADR) = 0x2100;
    REG(8, GRX_DMA_QWC) = 2;
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, 0x108);
    CHECK(o[0][0] == 0x100 && o[1][0] == 0 && o[2][0] == 0x300, "fromSPR interleave");
    CHECK(REG(8, GRX_DMA_MADR) == A(o + 4), "fromSPR interleave MADR 0x%X", REG(8, GRX_DMA_MADR));
    /* toSPR chain with TTE: the tags go to the scratchpad too */
    tag(t[0], 1, CNT, 0, 0, 0x77, 0x88);
    mark(t[1], 0xAA0);
    tag(t[2], 1, END, 0, 0, 0, 0);
    mark(t[3], 0xBB0);
    REG(9, GRX_DMA_TADR) = A(t);
    REG(9, GRX_DMA_SADR) = 0x2200;
    REG(9, GRX_DMA_QWC) = 0;
    grx_dma_write_chcr(GRX_DMA_TO_SPR, 0x144);
    CHECK(SPRP(0x2200)[2] == 0x77 && SPRP(0x2210)[0] == 0xAA0 && SPRP(0x2230)[0] == 0xBB0, "toSPR chain with TTE");
    CHECK(REG(9, GRX_DMA_SADR) == 0x2240, "toSPR chain SADR 0x%X", REG(9, GRX_DMA_SADR));
    /* fromSPR destination chain: cnt to o[4], end to o[6] */
    tag(SPRP(0x2300), 1, CNT, 0, A(o + 4), 0, 0);
    mark(SPRP(0x2310), 0xCC0);
    tag(SPRP(0x2320), 1, END, 0, A(o + 6), 0, 0);
    mark(SPRP(0x2330), 0xDD0);
    REG(8, GRX_DMA_SADR) = 0x2300;
    grx_dma_write_chcr(GRX_DMA_FROM_SPR, 0x104);
    CHECK(o[4][0] == 0xCC0 && o[6][0] == 0xDD0 && REG(8, GRX_DMA_SADR) == 0x2340, "fromSPR destination chain");

    /* A DMAC handler that starts the next GIF transfer */
    reset_fakes();
    on_raise = restart_gif;
    REG(2, GRX_DMA_MADR) = A(b);
    REG(2, GRX_DMA_QWC) = 1;
    grx_dma_write_chcr(GRX_DMA_GIF, 0x101);
    CHECK(gif_n == 2 && gif_q[1][0] == 0x200 && raised[2] == 2, "a transfer started from a handler (%d)", gif_n);
}

/* VIF0: MPG and UNPACK into VU0's memory in the mapped window */
static void test_vif0(void) {
    uint32_t *p = mem[0xB00];
    int n = 0;

    reset_fakes();
    p[n++] = 0x4A010000; /* MPG 1 pair to 0 */
    p[n++] = 0x8000033C;
    p[n++] = 0x400002FF;
    p[n++] = 0x01000404;
    p[n++] = 0x6C010020; /* UNPACK V4-32 to 0x20 */
    p[n++] = 1;
    p[n++] = 2;
    p[n++] = 3;
    p[n++] = 4;
    p[n++] = 0;
    p[n++] = 0;
    p[n++] = 0;
    REG(0, GRX_DMA_MADR) = A(p);
    REG(0, GRX_DMA_QWC) = n / 4;
    grx_dma_write_chcr(GRX_DMA_VIF0, 0x101);
    CHECK(GRX_HW32(0x11000000) == 0x8000033C && GRX_HW32(0x11000004) == 0x400002FF, "VIF0 MPG");
    CHECK(GRX_HW32(0x11004200) == 1 && GRX_HW32(0x1100420C) == 4, "VIF0 UNPACK");
    CHECK(raised[0] == 1, "VIF0 raised");
}

/* GS CSR stores: event bits cleared by 1s, RESET resets, the rest kept */
static void test_gs_csr(void) {
    gs_resets = 0;
    GRX_HW64(GRX_GS_CSR) = 0x551B0000u | 1u << 13 | 1u << 12 | 0x1F; /* FIELD, NFIELD, all events */
    grx_gs_csr_write(8); /* clear VSINT */
    CHECK(GRX_HW64(GRX_GS_CSR) == (0x551B0000u | 1u << 13 | 1u << 12 | 0x17), "VSINT cleared: 0x%llX",
          (unsigned long long)GRX_HW64(GRX_GS_CSR));
    grx_gs_csr_write(0x100 | 0x3000); /* FLUSH; read-only bits written */
    CHECK(GRX_HW64(GRX_GS_CSR) == (0x551B0000u | 1u << 13 | 1u << 12 | 0x17) && gs_resets == 0,
          "FLUSH and read-only bits change nothing: 0x%llX", (unsigned long long)GRX_HW64(GRX_GS_CSR));
    grx_gs_csr_write(0x200);
    CHECK(gs_resets == 1 && GRX_HW64(GRX_GS_CSR) == (0x551B4000u | 1u << 13), "RESET: 0x%llX",
          (unsigned long long)GRX_HW64(GRX_GS_CSR));
}

int main(void) {
    if (grx_hwmem_init() != 0) {
        const GrxHwRange *r = grx_hwmem_range(GRX_HW_EE_REGS);
        if (!r->mapped || !grx_hwmem_range(GRX_HW_SPR)->mapped || !grx_hwmem_range(GRX_HW_VU_MEM)->mapped) {
            printf("the hardware windows didn't map\n");
            return 1;
        }
    }
    GRX_HW32(GRX_D_CTRL) = 1;
    test_normal_gif();
    test_chain_gif();
    test_chain_spr();
    test_vif_unpack();
    test_vif_vu();
    test_vif_split();
    test_mfifo();
    test_spr();
    test_vif0();
    test_gs_csr();
    printf("%d checks, %d failed\n", checks, failures);
    return failures;
}
