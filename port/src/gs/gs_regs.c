/* The GS's general registers, the vertex queue and the GIF: what
   grx_gs_gif() feeds in, as the GIF unpacks it for the GS. */
#include "gs_internal.h"
#include "grx_gs.h"
#include "grx_hwmem.h"

GsState gs;
const GsBackend *gs_backend;

void grx_gs_set_priv(void *base) {
    gs.priv = base;
}

static inline float u2f(uint32_t u) {
    float f;
    memcpy(&f, &u, 4);
    return f;
}
static inline uint32_t f2u(float f) {
    uint32_t u;
    memcpy(&u, &f, 4);
    return u;
}

void grx_gs_init(void) {
    gs_mem_init();
    gs_queue_init();
    gs_flush();
    memset(gs_vram, 0, sizeof(gs_vram));
    if (gs_backend)
        gs_backend->loaded();
    if (!gs.priv)
        gs.priv = (uint8_t *)(uintptr_t)GRX_HW_GS_PRIV_BASE;
    grx_gs_reset();
}

void grx_gs_reset(void) {
    uint8_t *priv = gs.priv;
    uint64_t csr;

    if (gs_rec_active)
        gs_rec_event('R');
    gs_mem_init();
    memset(&gs, 0, sizeof(gs));
    gs.priv = priv ? priv : (uint8_t *)(uintptr_t)GRX_HW_GS_PRIV_BASE;
    gs.prmodecont = 1;
    gs.q = 1.0f;
    gs.rgbaq = (uint64_t)f2u(1.0f) << 32;
    gs.dirty = 1;
    /* CSR: RESET and the interrupt bits clear, FIFO empty, REV 0x1B, ID 0x55 */
    csr = gs_priv_read(GS_PRIV_CSR);
    csr &= (1u << 13); /* FIELD keeps going */
    csr |= (1u << 14) | (0x1Bu << 16) | (0x55u << 24);
    gs_priv_write(GS_PRIV_CSR, csr);
}

void grx_gs_gif_reset(void) {
    if (gs_rec_active)
        gs_rec_event('X');
    memset(gs.path, 0, sizeof(gs.path));
}

void grx_gs_vsync(void) {
    uint64_t csr;

    gs_rec_vsync();
    if (gs_backend)
        gs_backend->vsync();
    else
        gs_queue_vsync();
    csr = gs_priv_read(GS_PRIV_CSR);
    csr ^= 1u << 13;          /* FIELD */
    csr |= (1u << 3) | (1u << 12); /* VSINT, NFIELD */
    gs_priv_write(GS_PRIV_CSR, csr);
}

/* Vertices a primitive takes: point, line, line strip, triangle, strip,
   fan, sprite, (reserved) */
static const int prim_nverts[8] = {1, 2, 2, 3, 3, 3, 2, 0};

static uint32_t prim_ctxt(void) {
    uint64_t attr = (gs.prmodecont & 1) ? gs.prim : gs.prmode;
    return GS_FIELD(attr, 9, 1);
}

static void vertex_kick(uint64_t xyz, int has_f, int draw) {
    uint32_t type = GS_FIELD(gs.prim, 0, 3);
    const GsContext *c = &gs.ctx[prim_ctxt()];
    GsVertex *v;
    int need = prim_nverts[type];

    if (has_f)
        gs.fog = xyz & 0xFF00000000000000ull;
    if (!need)
        return;
    v = &gs.vq[gs.nv];
    v->x = (int32_t)(xyz & 0xFFFF) - (int32_t)GS_FIELD(c->xyoffset, 0, 16);
    v->y = (int32_t)((xyz >> 16) & 0xFFFF) - (int32_t)GS_FIELD(c->xyoffset, 32, 16);
    v->z = has_f ? GS_FIELD(xyz, 32, 24) : (uint32_t)(xyz >> 32);
    v->rgba = (uint32_t)gs.rgbaq;
    v->q = u2f((uint32_t)(gs.rgbaq >> 32));
    v->s = u2f((uint32_t)gs.st);
    v->t = u2f((uint32_t)(gs.st >> 32));
    v->u = (int32_t)GS_FIELD(gs.uv, 0, 14);
    v->v = (int32_t)GS_FIELD(gs.uv, 16, 14);
    v->f = GS_FIELD(gs.fog, 56, 8);
    if (++gs.nv < need)
        return;
    if (draw) {
        GsVertex wv[3];
        const GsVertex *dv = gs_ws_on ? gs_ws_prim(gs.vq, type, wv) : gs.vq;
        if (gs_backend)
            gs_backend->prim(dv, type);
        else
            gs_draw_prim(dv, type);
    }
    switch (type) {
    case 2: /* line strip */
        gs.vq[0] = gs.vq[1];
        gs.nv = 1;
        break;
    case 4: /* triangle strip */
        gs.vq[0] = gs.vq[1];
        gs.vq[1] = gs.vq[2];
        gs.nv = 2;
        break;
    case 5: /* fan: the first vertex stays */
        gs.vq[1] = gs.vq[2];
        gs.nv = 2;
        break;
    default:
        gs.nv = 0;
        break;
    }
}

static void signal_csr(uint32_t bit) {
    gs_priv_write(GS_PRIV_CSR, gs_priv_read(GS_PRIV_CSR) | bit);
}

void gs_write_reg(uint32_t addr, uint64_t d) {
    GsContext *c;

    switch (addr) {
    case GS_PRIM:
        /* The draw setup depends on the attributes, not the type */
        if ((d ^ gs.prim) & 0x7F8)
            gs.dirty = 1;
        gs.prim = d & 0x7FF;
        gs.nv = 0;
        return;
    case GS_RGBAQ: gs.rgbaq = d; return;
    case GS_ST: gs.st = d; return;
    case GS_UV: gs.uv = d; return;
    case GS_XYZF2: vertex_kick(d, 1, 1); return;
    case GS_XYZ2: vertex_kick(d, 0, 1); return;
    case GS_XYZF3: vertex_kick(d, 1, 0); return;
    case GS_XYZ3: vertex_kick(d, 0, 0); return;
    case GS_FOG: gs.fog = d; return;
    case GS_HWREG: {
        uint8_t b[8];
        memcpy(b, &d, 8);
        gs_trx_data(b, 8);
        return;
    }
    default: break;
    }
    gs.dirty = 1;
    switch (addr) {
    case GS_TEX0_1:
    case GS_TEX0_2:
        c = &gs.ctx[addr - GS_TEX0_1];
        c->tex0 = d;
        gs_clut_load(d);
        break;
    case GS_TEX2_1:
    case GS_TEX2_2: {
        /* PSM and the CLUT fields only */
        const uint64_t m = (0x3Full << 20) | (~0ull << 37);
        c = &gs.ctx[addr - GS_TEX2_1];
        c->tex0 = (c->tex0 & ~m) | (d & m);
        gs_clut_load(c->tex0);
        break;
    }
    case GS_CLAMP_1: case GS_CLAMP_2: gs.ctx[addr - GS_CLAMP_1].clamp = d; break;
    case GS_TEX1_1: case GS_TEX1_2: gs.ctx[addr - GS_TEX1_1].tex1 = d; break;
    case GS_XYOFFSET_1: case GS_XYOFFSET_2: gs.ctx[addr - GS_XYOFFSET_1].xyoffset = d; break;
    case GS_PRMODECONT: gs.prmodecont = d & 1; break;
    case GS_PRMODE: gs.prmode = d & 0x7F8; break;
    case GS_TEXCLUT: gs.texclut = d; break;
    case GS_SCANMSK: gs.scanmsk = d & 3; break;
    case GS_MIPTBP1_1: case GS_MIPTBP1_2: gs.ctx[addr - GS_MIPTBP1_1].miptbp1 = d; break;
    case GS_MIPTBP2_1: case GS_MIPTBP2_2: gs.ctx[addr - GS_MIPTBP2_1].miptbp2 = d; break;
    case GS_TEXA: gs.texa = d; break;
    case GS_FOGCOL: gs.fogcol = d; break;
    case GS_TEXFLUSH: break;
    case GS_SCISSOR_1: case GS_SCISSOR_2: gs.ctx[addr - GS_SCISSOR_1].scissor = d; break;
    case GS_ALPHA_1: case GS_ALPHA_2: gs.ctx[addr - GS_ALPHA_1].alpha = d; break;
    case GS_DIMX: gs.dimx = d; break;
    case GS_DTHE: gs.dthe = d & 1; break;
    case GS_COLCLAMP: gs.colclamp = d & 1; break;
    case GS_TEST_1: case GS_TEST_2: gs.ctx[addr - GS_TEST_1].test = d; break;
    case GS_PABE: gs.pabe = d & 1; break;
    case GS_FBA_1: case GS_FBA_2: gs.ctx[addr - GS_FBA_1].fba = d & 1; break;
    case GS_FRAME_1: case GS_FRAME_2: gs.ctx[addr - GS_FRAME_1].frame = d; break;
    case GS_ZBUF_1: case GS_ZBUF_2: gs.ctx[addr - GS_ZBUF_1].zbuf = d; break;
    case GS_BITBLTBUF: gs.bitbltbuf = d; break;
    case GS_TRXPOS: gs.trxpos = d; break;
    case GS_TRXREG: gs.trxreg = d; break;
    case GS_TRXDIR:
        gs.trxdir = d;
        gs_trx_start();
        break;
    case GS_SIGNAL: {
        uint64_t sl = gs_priv_read(GS_PRIV_SIGLBLID);
        uint32_t id = (uint32_t)d, mask = (uint32_t)(d >> 32);
        uint32_t sig = ((uint32_t)sl & ~mask) | (id & mask);
        gs_priv_write(GS_PRIV_SIGLBLID, (sl & 0xFFFFFFFF00000000ull) | sig);
        signal_csr(1u << 0);
        break;
    }
    case GS_FINISH:
        signal_csr(1u << 1);
        break;
    case GS_LABEL: {
        uint64_t sl = gs_priv_read(GS_PRIV_SIGLBLID);
        uint32_t id = (uint32_t)d, mask = (uint32_t)(d >> 32);
        uint32_t lbl = ((uint32_t)(sl >> 32) & ~mask) | (id & mask);
        gs_priv_write(GS_PRIV_SIGLBLID, (sl & 0xFFFFFFFFull) | ((uint64_t)lbl << 32));
        break;
    }
    default:
        break;
    }
}

/* One PACKED qword for register descriptor reg */
static void gif_packed(uint32_t reg, const uint8_t *p) {
    uint32_t w[4];

    memcpy(w, p, 16);
    switch (reg) {
    case 0x0: gs_write_reg(GS_PRIM, w[0] & 0x7FF); break;
    case 0x1:
        gs.rgbaq = (w[0] & 0xFF) | ((w[1] & 0xFF) << 8) | ((w[2] & 0xFF) << 16) | ((uint64_t)(w[3] & 0xFF) << 24) |
                   ((uint64_t)f2u(gs.q) << 32);
        break;
    case 0x2:
        gs.st = w[0] | ((uint64_t)w[1] << 32);
        gs.q = u2f(w[2]);
        break;
    case 0x3: gs.uv = (w[0] & 0x3FFF) | ((w[1] & 0x3FFF) << 16); break;
    case 0x4:
    case 0xC: {
        uint64_t v = (w[0] & 0xFFFF) | ((w[1] & 0xFFFF) << 16) | ((uint64_t)((w[2] >> 4) & 0xFFFFFF) << 32) |
                     ((uint64_t)((w[3] >> 4) & 0xFF) << 56);
        vertex_kick(v, 1, reg == 0x4 && !((w[3] >> 15) & 1));
        break;
    }
    case 0x5:
    case 0xD: {
        uint64_t v = (w[0] & 0xFFFF) | ((w[1] & 0xFFFF) << 16) | ((uint64_t)w[2] << 32);
        vertex_kick(v, 0, reg == 0x5 && !((w[3] >> 15) & 1));
        break;
    }
    case 0x6: case 0x7: case 0x8: case 0x9:
        gs_write_reg(reg, w[0] | ((uint64_t)w[1] << 32));
        break;
    case 0xA: gs.fog = (uint64_t)((w[3] >> 4) & 0xFF) << 56; break;
    case 0xE: gs_write_reg(w[2] & 0xFF, w[0] | ((uint64_t)w[1] << 32)); break;
    default: break; /* 0xB reserved, 0xF NOP */
    }
}

void grx_gs_gif(const void *data, uint32_t qwc, int path) {
    const uint8_t *p = data;
    GsGifPath *g = &gs.path[path & 3];

    if (gs_rec_active)
        gs_rec_gif(data, qwc, path);
    gs_ws_3d = g->ws3d;
    gs_ws_fill = g->wsfill;
    while (qwc) {
        if (g->nloop == 0) {
            uint64_t lo, hi;
            memcpy(&lo, p, 8);
            memcpy(&hi, p + 8, 8);
            p += 16;
            qwc--;
            g->nloop = (uint32_t)(lo & 0x7FFF);
            g->flg = GS_FIELD(lo, 58, 2);
            g->nreg = GS_FIELD(lo, 60, 4);
            if (!g->nreg)
                g->nreg = 16;
            g->regs = hi;
            g->curreg = 0;
            /* widescreen: VU1's packets and those the port marks are 3D */
            g->ws3d = (path & 3) == GRX_GIF_PATH1 || GS_FIELD(lo, 44, 1);
            gs_ws_3d = g->ws3d;
            g->wsfill = g->ws3d && gs_ws_k != 1.0f && g->flg == 0 ? gs_ws_tag_fills(lo, hi, p, qwc) : 0;
            gs_ws_fill = g->wsfill;
            gs.q = 1.0f;
            if (GS_FIELD(lo, 46, 1) && g->flg == 0)
                gs_write_reg(GS_PRIM, GS_FIELD(lo, 47, 11));
            continue;
        }
        switch (g->flg) {
        case 0: /* PACKED */
            while (qwc && g->nloop) {
                gif_packed((uint32_t)(g->regs >> (g->curreg * 4)) & 15, p);
                p += 16;
                qwc--;
                if (++g->curreg == g->nreg) {
                    g->curreg = 0;
                    g->nloop--;
                }
            }
            break;
        case 1: { /* REGLIST: two registers a qword, the last one padded */
            int half;
            for (half = 0; half < 2 && g->nloop; half++) {
                uint64_t d;
                uint32_t reg = (uint32_t)(g->regs >> (g->curreg * 4)) & 15;
                memcpy(&d, p + half * 8, 8);
                if (reg < 0xE)
                    gs_write_reg(reg, d);
                if (++g->curreg == g->nreg) {
                    g->curreg = 0;
                    g->nloop--;
                }
            }
            p += 16;
            qwc--;
            break;
        }
        default: { /* IMAGE */
            uint32_t n = g->nloop < qwc ? g->nloop : qwc;
            gs_trx_data(p, n * 16);
            p += n * 16;
            qwc -= n;
            g->nloop -= n;
            break;
        }
        }
    }
}
