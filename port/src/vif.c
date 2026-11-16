/*
 * The port's VIF0 and VIF1: interpreters for the VIF command stream that
 * the VIF DMA channels (port/src/dma.c) feed. See port/include/grx_vif.h.
 *
 * The stream is processed word by word as it arrives. A command whose data
 * is incomplete (UNPACK, MPG, STMASK/STROW/STCOL) keeps its words in a
 * buffer until the rest comes, so a command split across DMA tags or MFIFO
 * sends works; DIRECT's data is passed on to the GS as it comes, in whole
 * quadwords.
 *
 * VIF registers the game reads (VIFn_MARK, CYCLE, ..., STAT's DBF bit) are
 * mirrored into the mapped page at 0x10003800/0x10003C00 when they change.
 *
 * Not modelled: the VIF FIFO and stalls (the i bit, STAT's interrupt
 * stall, MSKPATH3's effect on the GIF: all logged or only tracked), VIF
 * error codes (treated as NOP, logged), reading from VIF1 (FIFO
 * direction "VIF1 to memory", sceGsExecStoreImage).
 */
#include <stdio.h>
#include <string.h>
#include "grx_vif.h"
#include "grx_vu1.h"
#include "grx_gs.h"
#include "grx_hwmem.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

uint8_t grx_vu1_data[GRX_VU1_DATA_SIZE] __attribute__((aligned(16)));
uint8_t grx_vu1_micro[GRX_VU1_MICRO_SIZE] __attribute__((aligned(16)));

/* VU0's memories, in the mapped window (grx_hwmem.h) */
#define VU0_MICRO 0x11000000u
#define VU0_DATA 0x11004000u
#define VU0_SIZE 0x1000u

/* VIFn register offsets in the page */
enum {
    R_STAT = 0x00,
    R_FBRST = 0x10,
    R_ERR = 0x20,
    R_MARK = 0x30,
    R_CYCLE = 0x40,
    R_MODE = 0x50,
    R_NUM = 0x60,
    R_MASK = 0x70,
    R_CODE = 0x80,
    R_ITOPS = 0x90,
    R_BASE = 0xA0,
    R_OFST = 0xB0,
    R_TOPS = 0xC0,
    R_ITOP = 0xD0,
    R_TOP = 0xE0,
    R_R0 = 0x100,
    R_C0 = 0x140
};

#define STAT_MRK 0x40u
#define STAT_DBF 0x80u

/* The largest command payload: UNPACK of 256 V4-32 vectors */
#define MAX_PAYLOAD 1024

typedef struct Vif {
    int id;
    uint32_t regs; /* VIFn's registers in the page */
    uint8_t *data; /* VU data memory */
    uint32_t data_qw_mask;
    uint8_t *micro; /* VU micro memory */
    uint32_t micro_mask;

    /* registers */
    uint32_t cl, wl, mode, mask, row[4], col[4];
    uint32_t itops, itop, base, ofst, tops, top, dbf;
    uint32_t mark, mskpath3;

    /* decode state */
    uint32_t code;        /* the command whose data is being collected */
    uint32_t need;        /* its data words still to come */
    uint32_t have;        /* words collected in buf */
    uint32_t buf[MAX_PAYLOAD];
    uint32_t direct_left; /* DIRECT quadwords still to pass on */
    uint32_t qbuf[4];     /* a DIRECT quadword split across writes */
    uint32_t qhave;

    /* a started microprogram, run at the next wait (grx_vif.h) */
    int pending;
    uint32_t pending_addr;
} Vif;

static Vif vifs[2];
static int inited;
static int trace;

static void vif_init(void) {
    int i;

    if (inited) {
        return;
    }
    inited = 1;
    for (i = 0; i < 2; i++) {
        memset(&vifs[i], 0, sizeof vifs[i]);
        vifs[i].id = i;
        vifs[i].regs = i ? 0x10003C00u : 0x10003800u;
    }
    vifs[1].data = grx_vu1_data;
    vifs[1].data_qw_mask = GRX_VU1_DATA_SIZE / 16 - 1;
    vifs[1].micro = grx_vu1_micro;
    vifs[1].micro_mask = GRX_VU1_MICRO_SIZE - 1;
    vifs[0].data_qw_mask = VU0_SIZE / 16 - 1;
    vifs[0].micro_mask = VU0_SIZE - 1;
}

/* VU0's memory is the mapped window, if it is mapped (tests may not) */
static int vu0_mem(Vif *v) {
    if (v->data == NULL && grx_hwmem_mapped(VU0_MICRO, 0x8000)) {
        v->micro = (uint8_t *)(uintptr_t)VU0_MICRO;
        v->data = (uint8_t *)(uintptr_t)VU0_DATA;
    }
    return v->data != NULL;
}

static void set_reg(Vif *v, uint32_t off, uint32_t val) {
    static int page;
    if (!page) {
        page = grx_hwmem_mapped(0x10003800u, 0x800);
    }
    if (page) {
        GRX_HW32(v->regs + off) = val;
    }
}

/* Each message once per VIF */
static void log_once(Vif *v, unsigned *seen, unsigned bit, const char *what, uint32_t code) {
    if (!(*seen & (bit << v->id))) {
        *seen |= bit << v->id;
        grx_log("vif%d: %s (code 0x%08X); further ones not reported", v->id, what, code);
    }
}

static unsigned seen_msgs;
enum { MSG_IBIT = 1, MSG_UNKNOWN = 4, MSG_VU0_MSCAL = 16, MSG_VU0_MEM = 64, MSG_VIF0_ONLY1 = 256,
       MSG_V_5 = 1024, MSG_WL0 = 4096 };

/* ---------------------------------------------------------------------- */
/* The microprogram */

static void run_pending(Vif *v) {
    uint32_t addr;

    if (!v->pending) {
        return;
    }
    v->pending = 0;
    addr = v->pending_addr;
    if (v->id == 1) {
        if (trace) {
            grx_log("vif1: VU1 runs from %s0x%X (TOP 0x%X, ITOP 0x%X)", addr == GRX_VU1_CONTINUE ? "MSCNT " : "",
                    addr == GRX_VU1_CONTINUE ? 0 : addr, v->top, v->itop);
        }
        grx_vu1_run(addr);
    } else {
        log_once(v, &seen_msgs, MSG_VU0_MSCAL, "VU0 microprogram started from VIF0, not run", addr);
    }
}

/* MSCAL/MSCALF/MSCNT, after the previous program has run */
static void start_program(Vif *v, uint32_t addr) {
    v->itop = v->itops;
    set_reg(v, R_ITOP, v->itop);
    if (v->id == 1) {
        v->top = v->tops;
        if (v->dbf) {
            v->tops = v->base;
            v->dbf = 0;
        } else {
            v->tops = (v->base + v->ofst) & 0x3FF;
            v->dbf = 1;
        }
        set_reg(v, R_TOP, v->top);
        set_reg(v, R_TOPS, v->tops);
        set_reg(v, R_STAT, (GRX_HW32(v->regs + R_STAT) & ~STAT_DBF) | (v->dbf ? STAT_DBF : 0));
    }
    v->pending = 1;
    v->pending_addr = addr;
}

uint32_t grx_vif1_top(void) {
    return vifs[1].top;
}

uint32_t grx_vif1_itop(void) {
    return vifs[1].itop;
}

int grx_vif1_path3_masked(void) {
    return (int)vifs[1].mskpath3;
}

int grx_vif_busy(int vif) {
    Vif *v = &vifs[vif & 1];
    return v->need != 0 || v->direct_left != 0;
}

void grx_vif_set_trace(int on) {
    trace = on;
}

/* ---------------------------------------------------------------------- */
/* UNPACK */

/* Input vectors an UNPACK of num writes reads: all of them, except in
   fill mode (WL > CL), where WL - CL of every WL are filled */
static uint32_t unpack_inputs(uint32_t num, uint32_t cl, uint32_t wl) {
    uint32_t rest;
    if (wl <= cl) {
        return num;
    }
    rest = num % wl;
    return num / wl * cl + (rest < cl ? rest : cl);
}

static uint32_t unpack_bits(uint32_t vn, uint32_t vl) {
    return vl == 3 ? 16 : (32u >> vl) * (vn + 1);
}

/* The effective cycle (STCYCL 0,x is undefined; treated as 1,1) */
static void unpack_cycle(Vif *v, uint32_t *cl, uint32_t *wl) {
    *cl = v->cl;
    *wl = v->wl;
    if (*wl == 0) {
        log_once(v, &seen_msgs, MSG_WL0, "UNPACK with STCYCL WL 0, done as CL = WL", 0);
        *wl = *cl ? *cl : 1;
        *cl = *wl;
    }
}

static uint32_t unpack_words(Vif *v, uint32_t code) {
    uint32_t cmd = (code >> 24) & 0x7F;
    uint32_t num = (code >> 16) & 0xFF;
    uint32_t cl, wl;

    unpack_cycle(v, &cl, &wl);
    if (num == 0) {
        num = 256;
    }
    return (unpack_inputs(num, cl, wl) * unpack_bits((cmd >> 2) & 3, cmd & 3) + 31) / 32;
}

/* One element of the payload: element i of size 32 >> vl bits */
static uint32_t unpack_elem(const Vif *v, uint32_t i, uint32_t vl, int usn, uint32_t nwords) {
    const uint8_t *b = (const uint8_t *)v->buf;
    switch (vl) {
    case 0:
        return i < nwords ? v->buf[i] : 0;
    case 1:
        if (i * 2 + 2 > nwords * 4) {
            return 0;
        }
        return usn ? (uint32_t)(b[i * 2] | b[i * 2 + 1] << 8) : (uint32_t)(int32_t)(int16_t)(b[i * 2] | b[i * 2 + 1] << 8);
    default:
        if (i + 1 > nwords * 4) {
            return 0;
        }
        return usn ? (uint32_t)b[i] : (uint32_t)(int32_t)(int8_t)b[i];
    }
}

static void do_unpack(Vif *v, uint32_t code) {
    uint32_t cmd = (code >> 24) & 0x7F;
    uint32_t imm = code & 0xFFFF;
    uint32_t vn = (cmd >> 2) & 3, vl = cmd & 3;
    int masked = (cmd & 0x10) != 0;
    int usn = (imm & 0x4000) != 0;
    uint32_t num = (code >> 16) & 0xFF;
    uint32_t addr = imm & 0x3FF;
    uint32_t nwords = v->have;
    uint32_t cl, wl, i, cycle = 0, in = 0;

    if (v->id == 1 && (imm & 0x8000)) {
        addr += v->tops;
    }
    if (num == 0) {
        num = 256;
    }
    unpack_cycle(v, &cl, &wl);
    if (vl == 3 && vn != 3) {
        log_once(v, &seen_msgs, MSG_V_5, "UNPACK of an undefined format (V?-5 other than V4-5), skipped", code);
        return;
    }
    if (v->data == NULL && !(v->id == 0 && vu0_mem(v))) {
        log_once(v, &seen_msgs, MSG_VU0_MEM, "VU0 memory isn't mapped, UNPACK/MPG dropped", code);
        return;
    }
    if (trace) {
        grx_log("vif%d: UNPACK %s%s-%d num %u to 0x%X%s%s cycle %u,%u mode %u", v->id, masked ? "masked " : "",
                vn == 0 ? "S" : vn == 1 ? "V2" : vn == 2 ? "V3" : "V4", vl == 3 ? 5 : 32 >> vl, num, addr,
                usn ? " usn" : "", (imm & 0x8000) ? " +TOPS" : "", cl, wl, v->mode);
    }
    for (i = 0; i < num; i++) {
        uint32_t *dst = (uint32_t *)(v->data + ((addr & v->data_qw_mask) << 4));
        uint32_t d[4] = {0, 0, 0, 0};
        int has_data = wl <= cl || cycle < cl;
        uint32_t r = cycle < 3 ? cycle : 3;
        int e;

        if (has_data) {
            if (vl == 3) {
                uint32_t p = unpack_elem(v, in, 1, 1, nwords);
                d[0] = (p & 0x1F) << 3;
                d[1] = ((p >> 5) & 0x1F) << 3;
                d[2] = ((p >> 10) & 0x1F) << 3;
                d[3] = ((p >> 15) & 1) << 7;
            } else {
                uint32_t base = in * (vn + 1);
                switch (vn) {
                case 0: /* S: x x x x */
                    d[0] = d[1] = d[2] = d[3] = unpack_elem(v, base, vl, usn, nwords);
                    break;
                case 1: /* V2: x y x y, as PCSX2 has it (z, w are undefined) */
                    d[0] = d[2] = unpack_elem(v, base, vl, usn, nwords);
                    d[1] = d[3] = unpack_elem(v, base + 1, vl, usn, nwords);
                    break;
                case 2: /* V3: w is whatever comes next in the stream */
                    d[0] = unpack_elem(v, base, vl, usn, nwords);
                    d[1] = unpack_elem(v, base + 1, vl, usn, nwords);
                    d[2] = unpack_elem(v, base + 2, vl, usn, nwords);
                    d[3] = unpack_elem(v, base + 3, vl, usn, nwords);
                    break;
                default:
                    for (e = 0; e < 4; e++) {
                        d[e] = unpack_elem(v, base + e, vl, usn, nwords);
                    }
                    break;
                }
            }
            in++;
        }
        for (e = 0; e < 4; e++) {
            uint32_t m = masked ? (v->mask >> (r * 8 + e * 2)) & 3 : 0;
            if (m == 0 && !has_data) {
                m = 1; /* a filled quadword: the row */
            }
            switch (m) {
            case 0:
                if (v->mode == 1) {
                    dst[e] = d[e] + v->row[e];
                } else if (v->mode == 2) {
                    v->row[e] += d[e];
                    dst[e] = v->row[e];
                } else {
                    dst[e] = d[e];
                }
                break;
            case 1:
                dst[e] = v->row[e];
                break;
            case 2:
                dst[e] = v->col[r];
                break;
            default: /* write-protected */
                break;
            }
        }
        addr++;
        if (++cycle == wl) {
            cycle = 0;
            if (wl < cl) {
                addr += cl - wl;
            }
        }
    }
    if (v->mode == 2) {
        for (i = 0; i < 4; i++) {
            set_reg(v, R_R0 + i * 0x10, v->row[i]);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Commands */

/* A command whose data has all arrived */
static void finish(Vif *v) {
    uint32_t code = v->code;
    uint32_t cmd = (code >> 24) & 0x7F;
    uint32_t i;

    switch (cmd) {
    case 0x20: /* STMASK */
        v->mask = v->buf[0];
        set_reg(v, R_MASK, v->mask);
        break;
    case 0x30: /* STROW */
        for (i = 0; i < 4; i++) {
            v->row[i] = v->buf[i];
            set_reg(v, R_R0 + i * 0x10, v->row[i]);
        }
        break;
    case 0x31: /* STCOL */
        for (i = 0; i < 4; i++) {
            v->col[i] = v->buf[i];
            set_reg(v, R_C0 + i * 0x10, v->col[i]);
        }
        break;
    case 0x4A: { /* MPG */
        uint32_t at = (code & 0xFFFF) * 8;
        if (v->micro == NULL && !(v->id == 0 && vu0_mem(v))) {
            log_once(v, &seen_msgs, MSG_VU0_MEM, "VU0 memory isn't mapped, UNPACK/MPG dropped", code);
            break;
        }
        if (trace) {
            grx_log("vif%d: MPG %u pairs to 0x%X", v->id, v->have / 2, at);
        }
        for (i = 0; i < v->have; i++) {
            uint32_t b = (at + i * 4) & v->micro_mask;
            memcpy(v->micro + b, &v->buf[i], 4);
        }
        break;
    }
    default:
        do_unpack(v, code);
        break;
    }
    v->code = 0;
    v->have = 0;
}

/* A new VIF code */
static void command(Vif *v, uint32_t code) {
    uint32_t cmd = (code >> 24) & 0x7F;
    uint32_t imm = code & 0xFFFF;
    uint32_t num = (code >> 16) & 0xFF;

    set_reg(v, R_CODE, code);
    if (code & 0x80000000u) {
        log_once(v, &seen_msgs, MSG_IBIT, "VIF code with the interrupt bit; no stall or interrupt", code);
    }
    if (trace && (cmd & 0x60) != 0x60) {
        grx_log("vif%d: code 0x%08X", v->id, code);
    }
    if (v->id == 0 && (cmd == 0x02 || cmd == 0x03 || cmd == 0x06 || cmd == 0x11 || cmd == 0x13 || cmd == 0x50 ||
                       cmd == 0x51)) {
        log_once(v, &seen_msgs, MSG_VIF0_ONLY1, "a VIF1-only code on VIF0, ignored", code);
        return;
    }
    switch (cmd) {
    case 0x00: /* NOP */
        break;
    case 0x01: /* STCYCL */
        v->cl = imm & 0xFF;
        v->wl = (imm >> 8) & 0xFF;
        set_reg(v, R_CYCLE, imm);
        break;
    case 0x02: /* OFFSET */
        v->ofst = imm & 0x3FF;
        v->dbf = 0;
        v->tops = v->base;
        set_reg(v, R_OFST, v->ofst);
        set_reg(v, R_TOPS, v->tops);
        set_reg(v, R_STAT, GRX_HW32(v->regs + R_STAT) & ~STAT_DBF);
        break;
    case 0x03: /* BASE */
        v->base = imm & 0x3FF;
        set_reg(v, R_BASE, v->base);
        break;
    case 0x04: /* ITOP */
        v->itops = imm & 0x3FF;
        set_reg(v, R_ITOPS, v->itops);
        break;
    case 0x05: /* STMOD */
        v->mode = imm & 3;
        set_reg(v, R_MODE, v->mode);
        break;
    case 0x06: /* MSKPATH3 */
        v->mskpath3 = (imm >> 15) & 1;
        break;
    case 0x07: /* MARK */
        v->mark = imm;
        set_reg(v, R_MARK, imm);
        set_reg(v, R_STAT, GRX_HW32(v->regs + R_STAT) | STAT_MRK);
        break;
    case 0x10: /* FLUSHE */
    case 0x11: /* FLUSH */
    case 0x13: /* FLUSHA */
        run_pending(v);
        break;
    case 0x14: /* MSCAL */
    case 0x15: /* MSCALF */
        run_pending(v);
        start_program(v, imm);
        break;
    case 0x17: /* MSCNT */
        run_pending(v);
        start_program(v, GRX_VU1_CONTINUE);
        break;
    case 0x20: /* STMASK */
        v->code = code;
        v->need = 1;
        break;
    case 0x30: /* STROW */
    case 0x31: /* STCOL */
        v->code = code;
        v->need = 4;
        break;
    case 0x4A: /* MPG: waits for the VU to stop */
        run_pending(v);
        v->code = code;
        v->need = (num ? num : 256) * 2;
        set_reg(v, R_NUM, num);
        break;
    case 0x50: /* DIRECT */
    case 0x51: /* DIRECTHL */
        v->direct_left = imm ? imm : 0x10000;
        break;
    default:
        if ((cmd & 0x60) == 0x60) {
            v->code = code;
            v->need = unpack_words(v, code);
            set_reg(v, R_NUM, num);
            if (v->need == 0) {
                finish(v);
            }
        } else {
            log_once(v, &seen_msgs, MSG_UNKNOWN, "unknown VIF code, taken as NOP", code);
        }
        break;
    }
}

void grx_vif_write(int vif, const uint32_t *w, uint32_t n) {
    Vif *v;

    vif_init();
    v = &vifs[vif & 1];
    while (n > 0) {
        if (v->direct_left) {
            if (v->qhave == 0 && n >= 4) {
                uint32_t q = n / 4 < v->direct_left ? n / 4 : v->direct_left;
                grx_gs_gif(w, q, GRX_GIF_PATH2);
                w += q * 4;
                n -= q * 4;
                v->direct_left -= q;
                continue;
            }
            v->qbuf[v->qhave++] = *w++;
            n--;
            if (v->qhave == 4) {
                grx_gs_gif(v->qbuf, 1, GRX_GIF_PATH2);
                v->qhave = 0;
                v->direct_left--;
            }
            continue;
        }
        if (v->need) {
            uint32_t k = n < v->need ? n : v->need;
            memcpy(v->buf + v->have, w, k * 4);
            v->have += k;
            v->need -= k;
            w += k;
            n -= k;
            if (v->need == 0) {
                finish(v);
            }
            continue;
        }
        command(v, *w++);
        n--;
    }
}

void grx_vif_sync(int vif) {
    vif_init();
    run_pending(&vifs[vif & 1]);
}

void grx_vif_reset(int vif) {
    Vif *v;

    vif_init();
    v = &vifs[vif & 1];
    v->code = v->need = v->have = 0;
    v->direct_left = v->qhave = 0;
    v->pending = 0;
    v->dbf = 0;
    v->mark = 0;
    set_reg(v, R_STAT, 0);
    set_reg(v, R_MARK, 0);
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): VU1's memories and both VIFs (their
   memory pointers are this exe's statics and the VU window: the same in
   every run). A microprogram is never left pending between fields. */
void grx_vif_state_io(GrxState *s) {
    GRX_STATE_VAR(s, grx_vu1_data);
    GRX_STATE_VAR(s, grx_vu1_micro);
    GRX_STATE_VAR(s, vifs);
    GRX_STATE_VAR(s, inited);
    grx_vu1_state_io(s);
}
