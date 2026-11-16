/*
 * The port's DMA controller, done synchronously (port/include/grx_dma.h,
 * port/docs/hardware.md (b) 3).
 *
 * A start reads the channel's registers from the mapped page, runs the
 * whole transfer and writes the registers back as the DMAC leaves them at
 * the end: CHCR.STR clear (ASP and TAG updated in chain mode), QWC 0,
 * MADR/TADR/SADR advanced, D_STAT's CIS bit for the channel set. Then the
 * kernel runs the channel's DMAC handlers (grx_kernel_raise_dmac).
 *
 * Modes:
 * - normal: QWC quadwords from MADR (to SADR for toSPR, from SADR to MADR
 *   for fromSPR)
 * - source chain (VIF0, VIF1, GIF, toSPR): the tags refe, cnt, next, ref,
 *   refs, call, ret, end; the call stack in ASR0/ASR1 and CHCR.ASP; TTE
 *   (the tag's upper 64 bits go to the VIF ahead of the data, the whole tag
 *   to the scratchpad for toSPR); a tag with IRQ ends the transfer when
 *   CHCR.TIE is set. A start with QWC non-zero first finishes the
 *   quadwords left at MADR, as the DMAC does.
 * - destination chain (fromSPR): cnts, cnt and end tags in the scratchpad
 * - interleave (the scratchpad channels): D_SQWC's TQWC transferred, SQWC
 *   skipped, on the memory side
 * - MFIFO (D_CTRL.MFD = 2: VIF1, 3: GIF): fromSPR writes into the ring at
 *   D_RBOR/D_RBSR, then the drain channel (if started in chain mode) runs
 *   the tags in the ring at once, until its TADR reaches fromSPR's MADR
 *   (it then stays started, stalled, as on the hardware) or an end tag.
 *
 * Stall control (D_CTRL STS/STD, D_STADR), the DMAC's priority control
 * (D_PCR) and D_ENABLEW's suspend mean nothing when every transfer is done
 * at once, and are ignored. IPU (3/4) transfers are dropped, reported
 * once; SIF (5/6/7) is port/src/sif.c's (grx_dma_set_channel_hook).
 *
 * The other register-started work of the SDK that has no better home:
 * _sceSioPutRawChar's SIO output (grx_sio_putc), and stores to the GS's
 * CSR with the real register's write-1-to-clear and reset semantics
 * (grx_gs_csr_write).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_dma.h"
#include "grx_vif.h"
#include "grx_gs.h"
#include "grx_kernel.h"
#include "grx_hwmem.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

#define SPR ((uint8_t *)(uintptr_t)GRX_HW_SPR_BASE)
#define SPR_MASK 0x3FF0u

/* Tags in one transfer before it is taken for a loop */
#define MAX_TAGS 1000000

static const uint32_t chan_base[GRX_DMA_NCHANNELS] = {
    GRX_DMA_D0, GRX_DMA_D1, GRX_DMA_D2, GRX_DMA_D3, GRX_DMA_D4,
    GRX_DMA_D5, GRX_DMA_D6, GRX_DMA_D7, GRX_DMA_D8, GRX_DMA_D9,
};
static const char *const chan_name[GRX_DMA_NCHANNELS] = {
    "VIF0", "VIF1", "GIF", "fromIPU", "toIPU", "SIF0", "SIF1", "SIF2", "fromSPR", "toSPR",
};

static GrxDmaChannelHook hooks[GRX_DMA_NCHANNELS];
static int active[GRX_DMA_NCHANNELS];
static int trace;
static unsigned reported; /* one-time messages */

#define REG(ch, off) GRX_HW32(chan_base[ch] + (off))

uint32_t grx_dma_chan_base(int ch) {
    return ch >= 0 && ch < GRX_DMA_NCHANNELS ? chan_base[ch] : 0;
}

int grx_dma_channel(uintptr_t reg) {
    int ch;
    for (ch = 0; ch < GRX_DMA_NCHANNELS; ch++) {
        if (reg >= chan_base[ch] && reg < chan_base[ch] + 0x100) {
            return ch;
        }
    }
    return -1;
}

void grx_dma_set_channel_hook(int ch, GrxDmaChannelHook hook) {
    if (ch >= 0 && ch < GRX_DMA_NCHANNELS) {
        hooks[ch] = hook;
    }
}

void grx_dma_set_trace(int on) {
    trace = on;
}

static void report_once(unsigned bit, const char *fmt, int ch, uint32_t v) {
    if (!(reported & bit)) {
        reported |= bit;
        grx_log(fmt, chan_name[ch], v);
    }
}

/* ---------------------------------------------------------------------- */
/* Addresses */

/* The MFIFO ring, when a transfer reads (drain) or writes (fromSPR) it */
typedef struct Ring {
    int on;
    uint32_t base, mask; /* D_RBOR, D_RBSR: the ring is mask + 16 bytes */
} Ring;

static Ring ring_of(void) {
    Ring r;
    r.base = GRX_HW32(GRX_D_RBOR) & 0x0FFFFFF0u;
    r.mask = GRX_HW32(GRX_D_RBSR) & 0x0FFFFFF0u;
    r.on = 1;
    return r;
}

/* An address in the ring, wrapped into it. The offset is taken from the
   ring's start, so it works when the port's ring isn't aligned to its
   size (the hardware requires it to be) */
static uint32_t ring_wrap(const Ring *r, uint32_t a) {
    return r->base + (((a & 0x0FFFFFF0u) - r->base) & r->mask);
}

static int is_spr(uint32_t a) {
    return (a & 0x80000000u) != 0 || (a & 0xFFFFC000u) == GRX_HW_SPR_BASE;
}

/* a + n bytes, scratchpad addresses wrapping inside the scratchpad */
static uint32_t addr_add(uint32_t a, uint32_t n) {
    if (is_spr(a)) {
        return (a & ~0x3FFFu) | ((a + n) & 0x3FFFu);
    }
    return a + n;
}

/* The host memory for main memory address a (not the scratchpad) */
static uint8_t *mem_ptr(uint32_t a) {
    uint32_t p = a & 0x0FFFFFF0u;
    if (p < 0x10000u) {
        return NULL;
    }
    return (uint8_t *)(uintptr_t)p;
}

/* The host address of one quadword, and how many quadwords from it are
   contiguous (up to want), for an address in main memory, the scratchpad,
   or the ring */
static uint8_t *qw_ptr(uint32_t a, const Ring *ring, uint32_t want, uint32_t *contig) {
    if (is_spr(a)) {
        uint32_t off = a & SPR_MASK;
        uint32_t n = (0x4000u - off) / 16;
        *contig = n < want ? n : want;
        return SPR + off;
    }
    if (ring != NULL && ring->on) {
        uint32_t off = ((a & 0x0FFFFFF0u) - ring->base) & ring->mask;
        uint32_t n = (ring->mask + 16 - off) / 16;
        *contig = n < want ? n : want;
        return mem_ptr(ring->base + off);
    }
    *contig = want;
    return mem_ptr(a);
}

/* ---------------------------------------------------------------------- */
/* A transfer in progress */

typedef struct Xfer {
    int ch;
    uint32_t chcr;
    uint32_t sadr;  /* toSPR's destination */
    int failed;
} Xfer;

/* Data from memory to the channel's device */
static void sink(Xfer *x, const uint8_t *p, uint32_t qwc) {
    switch (x->ch) {
    case GRX_DMA_VIF0:
    case GRX_DMA_VIF1:
        grx_vif_write(x->ch, (const uint32_t *)p, qwc * 4);
        break;
    case GRX_DMA_GIF:
        grx_gs_gif(p, qwc, GRX_GIF_PATH3);
        break;
    case GRX_DMA_TO_SPR:
        while (qwc--) {
            memcpy(SPR + (x->sadr & SPR_MASK), p, 16);
            x->sadr = (x->sadr + 16) & 0x3FFFu;
            p += 16;
        }
        break;
    default:
        break;
    }
}

/* qwc quadwords from a (the ring's if in_ring) to the device; returns the
   address after them */
static uint32_t send(Xfer *x, uint32_t a, uint32_t qwc, const Ring *ring) {
    while (qwc > 0 && !x->failed) {
        uint32_t n;
        uint8_t *p = qw_ptr(a, ring, qwc, &n);
        if (p == NULL) {
            grx_log("dma: %s reads from a bad address 0x%08X (%u quadwords)", chan_name[x->ch], a, qwc);
            x->failed = 1;
            break;
        }
        if (trace) {
            grx_log("dma: %s %u quadwords from 0x%08X", chan_name[x->ch], n, a);
        }
        sink(x, p, n);
        qwc -= n;
        a = ring != NULL && ring->on ? ring_wrap(ring, a + n * 16) : addr_add(a, n * 16);
    }
    return a;
}

/* One quadword from the scratchpad (at SPR offset s) to memory at a (the
   ring's if it is on) */
static int spr_to_mem(uint32_t s, uint32_t a, const Ring *ring) {
    uint32_t n;
    uint8_t *d = qw_ptr(a, ring, 1, &n);
    if (d == NULL) {
        return -1;
    }
    memcpy(d, SPR + (s & SPR_MASK), 16);
    return 0;
}

/* The end of a transfer: STR off, QWC 0, the CIS bit, the handlers */
static void complete(int ch, uint32_t chcr) {
    REG(ch, GRX_DMA_QWC) = 0;
    REG(ch, GRX_DMA_CHCR) = chcr & ~GRX_CHCR_STR;
    GRX_HW32(GRX_D_STAT) |= 1u << ch;
    if (trace) {
        grx_log("dma: %s done", chan_name[ch]);
    }
    /* Its handlers may start it again */
    active[ch] = 0;
    grx_kernel_raise_dmac(ch);
}

/* ---------------------------------------------------------------------- */
/* Source chain (and the MFIFO drain, with ring set). Returns 1 when the
   chain ended, 0 when it stalled at fromSPR's MADR (MFIFO). The registers
   are written back either way; the caller completes the channel. */
static int source_chain(Xfer *x, const Ring *ring) {
    int ch = x->ch;
    uint32_t chcr = x->chcr;
    uint32_t tadr = REG(ch, GRX_DMA_TADR);
    uint32_t madr = REG(ch, GRX_DMA_MADR);
    uint32_t qwc = REG(ch, GRX_DMA_QWC) & 0xFFFFu;
    uint32_t asr[2];
    uint32_t asp = (chcr >> 4) & 3;
    int ended = 0, ntags = 0;
    int tte = (chcr & GRX_CHCR_TTE) != 0;
    const Ring *r = ring != NULL && ring->on ? ring : NULL;

    asr[0] = REG(ch, GRX_DMA_ASR0);
    asr[1] = REG(ch, GRX_DMA_ASR1);
    if (r != NULL) {
        tadr = ring_wrap(r, tadr);
    }

    /* A transfer stopped (or set up) with quadwords left: those first, then
       the tag that came with them decides how it goes on */
    if (qwc != 0) {
        uint32_t id = (chcr >> 28) & 7;
        madr = send(x, madr, qwc, NULL);
        qwc = 0;
        if (id == 0 || id == 7 || ((chcr >> 31) && (chcr & GRX_CHCR_TIE))) {
            ended = 1;
        }
    }

    while (!ended && !x->failed) {
        const uint32_t *tag;
        uint32_t n, lo, hi, id, addr, data, next;
        int from_ring = 0;

        if (r != NULL) {
            uint32_t wr = ring_wrap(r, GRX_HW32(GRX_DMA_D8 + GRX_DMA_MADR));
            if (tadr == wr) {
                break; /* stalled: the ring is empty */
            }
        }
        if (++ntags > MAX_TAGS) {
            grx_log("dma: %s chain at 0x%08X goes on for more than %d tags, stopped", chan_name[ch], tadr, MAX_TAGS);
            x->failed = 1;
            break;
        }
        tag = (const uint32_t *)qw_ptr(tadr, r, 1, &n);
        if (tag == NULL) {
            grx_log("dma: %s tag at a bad address 0x%08X", chan_name[ch], tadr);
            x->failed = 1;
            break;
        }
        lo = tag[0];
        hi = tag[1];
        qwc = lo & 0xFFFFu;
        id = (lo >> 28) & 7;
        addr = hi; /* ADDR in bits 0-30, SPR in bit 31 */
        chcr = (chcr & 0xFFFFu) | (lo & 0xFFFF0000u);
        if (trace) {
            static const char *const ids[8] = {"refe", "cnt", "next", "ref", "refs", "call", "ret", "end"};
            grx_log("dma: %s tag at 0x%08X: %s qwc %u addr 0x%08X%s", chan_name[ch], tadr, ids[id], qwc, addr,
                    (lo >> 31) ? " irq" : "");
        }
        if (tte) {
            if (ch == GRX_DMA_VIF0 || ch == GRX_DMA_VIF1) {
                grx_vif_write(ch, tag + 2, 2);
            } else if (ch == GRX_DMA_TO_SPR) {
                sink(x, (const uint8_t *)tag, 1);
            }
        }
        data = r != NULL ? ring_wrap(r, tadr + 16) : addr_add(tadr, 16);
        switch (id) {
        case 0: /* refe */
            data = addr;
            next = r != NULL ? ring_wrap(r, tadr + 16) : addr_add(tadr, 16);
            ended = 1;
            break;
        case 1: /* cnt */
            from_ring = 1;
            next = 0; /* after the data */
            break;
        case 2: /* next */
            from_ring = 1;
            next = r != NULL ? ring_wrap(r, addr) : addr;
            break;
        case 3: /* ref */
        case 4: /* refs */
            data = addr;
            next = r != NULL ? ring_wrap(r, tadr + 16) : addr_add(tadr, 16);
            break;
        case 5: /* call */
            from_ring = 1;
            if (asp >= 2) {
                grx_log("dma: %s call at 0x%08X with ASP %u, ended", chan_name[ch], tadr, asp);
                x->failed = 1;
                next = tadr;
                break;
            }
            asr[asp++] = r != NULL ? ring_wrap(r, data + qwc * 16) : addr_add(data, qwc * 16);
            next = addr;
            break;
        case 6: /* ret */
            from_ring = 1;
            if (asp > 0) {
                next = asr[--asp];
            } else {
                next = 0;
                ended = 1;
            }
            break;
        default: /* end */
            from_ring = 1;
            next = tadr;
            ended = 1;
            break;
        }
        if (x->failed) {
            break;
        }
        madr = send(x, data, qwc, from_ring ? r : NULL);
        if (id == 1 || (id == 6 && ended)) {
            next = madr;
        }
        tadr = next;
        qwc = 0;
        if ((lo >> 31) && (chcr & GRX_CHCR_TIE)) {
            ended = 1;
        }
    }
    REG(ch, GRX_DMA_TADR) = tadr;
    REG(ch, GRX_DMA_MADR) = madr;
    REG(ch, GRX_DMA_QWC) = 0;
    REG(ch, GRX_DMA_ASR0) = asr[0];
    REG(ch, GRX_DMA_ASR1) = asr[1];
    x->chcr = (chcr & ~GRX_CHCR_ASP) | (asp << 4);
    REG(ch, GRX_DMA_CHCR) = x->chcr;
    return ended || x->failed;
}

/* fromSPR's destination chain: cnts/cnt/end tags in the scratchpad at
   SADR, each followed by its data, which goes to the tag's ADDR */
static void dest_chain(Xfer *x) {
    uint32_t sadr = REG(GRX_DMA_FROM_SPR, GRX_DMA_SADR) & 0x3FFFu;
    uint32_t madr = REG(GRX_DMA_FROM_SPR, GRX_DMA_MADR);
    uint32_t chcr = x->chcr;
    int ntags = 0;

    for (;;) {
        const uint32_t *tag = (const uint32_t *)(SPR + (sadr & SPR_MASK));
        uint32_t lo = tag[0], qwc = lo & 0xFFFFu, id = (lo >> 28) & 7, i;

        chcr = (chcr & 0xFFFFu) | (lo & 0xFFFF0000u);
        madr = tag[1];
        sadr = (sadr + 16) & 0x3FFFu;
        if (id != 0 && id != 1 && id != 7) {
            grx_log("dma: fromSPR destination tag %u at SPR 0x%04X, ended", id, sadr - 16);
            break;
        }
        for (i = 0; i < qwc; i++) {
            if (spr_to_mem(sadr, madr, NULL) != 0) {
                grx_log("dma: fromSPR writes to a bad address 0x%08X", madr);
                x->failed = 1;
                break;
            }
            sadr = (sadr + 16) & 0x3FFFu;
            madr += 16;
        }
        if (x->failed || id == 7 || ((lo >> 31) && (chcr & GRX_CHCR_TIE)) || ++ntags > MAX_TAGS) {
            break;
        }
    }
    REG(GRX_DMA_FROM_SPR, GRX_DMA_SADR) = sadr;
    REG(GRX_DMA_FROM_SPR, GRX_DMA_MADR) = madr;
    x->chcr = chcr;
}

/* Normal and interleave mode for the scratchpad channels */
static void spr_block(Xfer *x, int interleave, const Ring *ring) {
    int ch = x->ch;
    uint32_t madr = REG(ch, GRX_DMA_MADR);
    uint32_t sadr = REG(ch, GRX_DMA_SADR) & 0x3FFFu;
    uint32_t qwc = REG(ch, GRX_DMA_QWC) & 0xFFFFu;
    uint32_t sq = GRX_HW32(GRX_D_SQWC);
    uint32_t tqwc = (sq >> 16) & 0xFF, sqwc = sq & 0xFF, run = 0;

    if (interleave && tqwc == 0) {
        grx_log("dma: %s interleave with TQWC 0, done as normal mode", chan_name[ch]);
        interleave = 0;
    }
    if (trace) {
        grx_log("dma: %s %u quadwords, memory 0x%08X, SPR 0x%04X%s", chan_name[ch], qwc, madr, sadr,
                interleave ? " interleaved" : "");
    }
    while (qwc > 0) {
        if (ch == GRX_DMA_FROM_SPR) {
            if (spr_to_mem(sadr, madr, ring) != 0) {
                grx_log("dma: fromSPR writes to a bad address 0x%08X", madr);
                break;
            }
        } else {
            uint32_t n;
            const uint8_t *p = qw_ptr(madr, NULL, 1, &n);
            if (p == NULL) {
                grx_log("dma: toSPR reads from a bad address 0x%08X", madr);
                break;
            }
            memcpy(SPR + (sadr & SPR_MASK), p, 16);
        }
        sadr = (sadr + 16) & 0x3FFFu;
        madr = ring != NULL && ring->on ? ring_wrap(ring, madr + 16) : addr_add(madr, 16);
        qwc--;
        if (interleave && ++run == tqwc) {
            run = 0;
            madr = addr_add(madr, sqwc * 16);
        }
    }
    REG(ch, GRX_DMA_MADR) = madr;
    REG(ch, GRX_DMA_SADR) = sadr;
}

/* ---------------------------------------------------------------------- */
/* Starting */

static int mfifo_drain_channel(void) {
    uint32_t mfd = (GRX_HW32(GRX_D_CTRL) >> 2) & 3;
    return mfd == 2 ? GRX_DMA_VIF1 : mfd == 3 ? GRX_DMA_GIF : -1;
}

static int draining;

/* The MFIFO's drain channel, started in chain mode: run what the ring
   holds. Called again from inside (a DMAC handler that sends more) it
   returns: the outer loop reads fromSPR's MADR each time round. */
static void drain(int ch) {
    Xfer x;
    Ring r;
    int ended;

    if (draining) {
        return;
    }
    if (!(REG(ch, GRX_DMA_CHCR) & GRX_CHCR_STR) || ((REG(ch, GRX_DMA_CHCR) >> 2) & 3) != 1) {
        return;
    }
    draining = 1;
    r = ring_of();
    memset(&x, 0, sizeof x);
    x.ch = ch;
    x.chcr = REG(ch, GRX_DMA_CHCR);
    ended = source_chain(&x, &r);
    if (ch == GRX_DMA_VIF1) {
        grx_vif_sync(1);
    }
    draining = 0;
    if (ended) {
        complete(ch, x.chcr);
    }
}

static void run(int ch) {
    Xfer x;
    uint32_t chcr = REG(ch, GRX_DMA_CHCR);
    uint32_t mod = (chcr >> 2) & 3;
    int drain_ch = mfifo_drain_channel();

    memset(&x, 0, sizeof x);
    x.ch = ch;
    x.chcr = chcr;

    if (!(GRX_HW32(GRX_D_CTRL) & 1)) {
        report_once(1, "dma: %s started with D_CTRL.DMAE off (0x%08X); run anyway", ch, GRX_HW32(GRX_D_CTRL));
    }
    if (hooks[ch] != NULL) {
        hooks[ch](ch);
        return;
    }
    if (trace) {
        grx_log("dma: start %s CHCR 0x%08X MADR 0x%08X QWC 0x%X TADR 0x%08X SADR 0x%04X", chan_name[ch], chcr,
                REG(ch, GRX_DMA_MADR), REG(ch, GRX_DMA_QWC), REG(ch, GRX_DMA_TADR), REG(ch, GRX_DMA_SADR));
    }
    switch (ch) {
    case GRX_DMA_VIF0:
    case GRX_DMA_VIF1:
    case GRX_DMA_GIF:
        if (ch == GRX_DMA_VIF1 && !(chcr & GRX_CHCR_DIR)) {
            report_once(2, "dma: %s from the VIF to memory (CHCR 0x%08X) isn't supported; nothing read", ch, chcr);
            REG(ch, GRX_DMA_MADR) += (REG(ch, GRX_DMA_QWC) & 0xFFFFu) * 16;
            break;
        }
        if (ch == drain_ch && mod == 1) {
            /* The MFIFO's drain: it runs whenever fromSPR adds to the ring */
            drain(ch);
            return;
        }
        if (mod == 1) {
            source_chain(&x, NULL);
        } else {
            uint32_t madr = REG(ch, GRX_DMA_MADR);
            REG(ch, GRX_DMA_MADR) = send(&x, madr, REG(ch, GRX_DMA_QWC) & 0xFFFFu, NULL);
        }
        if (ch != GRX_DMA_GIF) {
            grx_vif_sync(ch);
        }
        break;
    case GRX_DMA_FROM_SPR:
        if (mod == 1) {
            dest_chain(&x);
        } else {
            Ring r = ring_of();
            r.on = drain_ch >= 0;
            spr_block(&x, mod == 2, &r);
            if (r.on) {
                /* The ring has more: complete fromSPR first (its MADR is
                   what the drain stops at), then the drain */
                REG(ch, GRX_DMA_QWC) = 0;
                REG(ch, GRX_DMA_CHCR) = x.chcr & ~GRX_CHCR_STR;
                GRX_HW32(GRX_D_STAT) |= 1u << ch;
                drain(drain_ch);
                if (trace) {
                    grx_log("dma: %s done", chan_name[ch]);
                }
                active[ch] = 0;
                grx_kernel_raise_dmac(ch);
                return;
            }
        }
        break;
    case GRX_DMA_TO_SPR:
        if (mod == 1) {
            x.sadr = REG(ch, GRX_DMA_SADR) & 0x3FFFu;
            source_chain(&x, NULL);
            REG(ch, GRX_DMA_SADR) = x.sadr;
        } else {
            spr_block(&x, mod == 2, NULL);
        }
        break;
    case GRX_DMA_FROM_IPU:
    case GRX_DMA_TO_IPU:
        report_once(4, "dma: %s isn't supported yet (movies); a transfer dropped (CHCR 0x%08X)", ch, chcr);
        break;
    default:
        report_once(8, "dma: %s has no handler (port/src/sif.c sets one); a transfer dropped (CHCR 0x%08X)", ch,
                    chcr);
        break;
    }
    complete(ch, x.chcr);
}

void grx_dma_start(int ch) {
    if (ch < 0 || ch >= GRX_DMA_NCHANNELS) {
        grx_log("dma: start of channel %d", ch);
        return;
    }
    if (!(REG(ch, GRX_DMA_CHCR) & GRX_CHCR_STR)) {
        return;
    }
    if (active[ch]) {
        /* A handler restarting the channel that just finished is fine (its
           STR is clear by then); this is a start while it runs */
        grx_log("dma: %s started again while its transfer runs; ignored", chan_name[ch]);
        return;
    }
    active[ch] = 1;
    run(ch);
    active[ch] = 0;
}

void grx_dma_kick(volatile void *chan) {
    int ch = grx_dma_channel((uintptr_t)chan);
    if (ch < 0) {
        grx_log("dma: kick of 0x%08X, not a DMA channel", (unsigned)(uintptr_t)chan);
        return;
    }
    grx_dma_start(ch);
}

void grx_dma_write_chcr(int ch, uint32_t chcr) {
    if (ch < 0 || ch >= GRX_DMA_NCHANNELS) {
        grx_log("dma: CHCR store to channel %d", ch);
        return;
    }
    REG(ch, GRX_DMA_CHCR) = chcr;
    grx_dma_start(ch);
}

int grx_dma_wait(int ch) {
    if (ch < 0 || ch >= GRX_DMA_NCHANNELS || !(REG(ch, GRX_DMA_CHCR) & GRX_CHCR_STR)) {
        return 0;
    }
    grx_log("dma: waiting for %s, which can't finish (CHCR 0x%08X%s)", chan_name[ch], REG(ch, GRX_DMA_CHCR),
            ch == mfifo_drain_channel() ? ", the MFIFO drain" : "");
    return -1;
}

void grx_dma_reset(void) {
    int ch;
    /* GRX_DMA_TRACE=1: every transfer and tag in the log */
    const char *t = getenv("GRX_DMA_TRACE");
    if (t != NULL && *t != 0 && *t != '0') {
        trace = 1;
    }
    for (ch = 0; ch < GRX_DMA_NCHANNELS; ch++) {
        active[ch] = 0;
    }
    draining = 0;
    if (trace) {
        grx_log("dma: reset");
    }
}

/* ---------------------------------------------------------------------- */

void grx_gs_csr_write(uint64_t value) {
    volatile uint64_t *csr = (volatile uint64_t *)(uintptr_t)GRX_GS_CSR;

    if (value & 0x200) {
        grx_gs_reset();
    }
    *csr &= ~(value & 0x1Fu);
}

void grx_sio_putc(int c) {
    fputc(c, stderr);
    if (c == '\n') {
        fflush(stderr);
    }
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): between fields no channel is running;
   the channels' registers are in the EE register window, saved as memory */
const char *grx_dma_state_blocker(void) {
    int i;
    for (i = 0; i < GRX_DMA_NCHANNELS; i++) {
        if (active[i]) {
            return "a DMA transfer is running";
        }
    }
    return draining ? "a DMA transfer is running" : NULL;
}

void grx_dma_state_io(GrxState *s) {
    (void)s;
}
