/* Emulation of the R5900 instructions in the game's inline asm (see
   port/include/grx_ee.h). */
#include <string.h>
#include <stdlib.h>
#include "grx_ee.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

GrxQuad grx_vf[33] = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
unsigned int grx_vi[32];
GrxQuad grx_gpr[32];

/* $vf0 is the constant (0, 0, 0, 1): writes to it are dropped */
static void write_vf(int d, int mask, const float *v) {
    int i;
    if (d == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            grx_vf[d].f[i] = v[i];
        }
    }
}

void grx_lqc2(int vf, const void *addr) {
    if (vf != 0) {
        memcpy(&grx_vf[vf], addr, 16);
    }
}

void grx_sqc2(int vf, void *addr) {
    memcpy(addr, &grx_vf[vf], 16);
}

void grx_qmtc2_raw(int vf, const void *src, unsigned int size) {
    GrxQuad q;
    if (vf == 0) {
        return;
    }
    memset(&q, 0, sizeof(q));
    memcpy(&q, src, size < 16 ? size : 16);
    grx_vf[vf] = q;
}

void grx_qmfc2_raw(void *dst, int vf, unsigned int size) {
    memcpy(dst, &grx_vf[vf], size < 16 ? size : 16);
}

/* VI29 is VPU-STAT: the port's VU0 and VU1 are never busy */
unsigned int grx_cfc2(int vi) {
    return vi == 29 ? 0 : grx_vi[vi];
}

void grx_ctc2(int vi, unsigned int value) {
    if (vi != 0) {
        grx_vi[vi] = value;
    }
}

void grx_vu0_arith(int op, int mask, int d, int s, int t, int bc) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f, *acc = grx_vf[GRX_ACC].f;
    float r[4];
    int i;
    for (i = 0; i < 4; i++) {
        float y = b[bc >= 0 ? bc : i];
        switch (op) {
        case GRX_VADD:  r[i] = a[i] + y; break;
        case GRX_VSUB:  r[i] = a[i] - y; break;
        case GRX_VMUL:  r[i] = a[i] * y; break;
        case GRX_VMADD: r[i] = acc[i] + a[i] * y; break;
        case GRX_VMSUB: r[i] = acc[i] - a[i] * y; break;
        case GRX_VMAX:  r[i] = a[i] > y ? a[i] : y; break;
        case GRX_VMINI: r[i] = a[i] < y ? a[i] : y; break;
        default: grx_fatal("grx_vu0_arith: op %d", op);
        }
    }
    write_vf(d, mask, r);
}

/* Outer product: ACC.xyz = s.yzx * t.zxy; vopmsub subtracts the other half */
void grx_vu0_opmula(int s, int t) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f;
    float r[4];
    r[0] = a[1] * b[2];
    r[1] = a[2] * b[0];
    r[2] = a[0] * b[1];
    r[3] = 0.0f;
    write_vf(GRX_ACC, 7, r);
}

void grx_vu0_opmsub(int d, int s, int t) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f, *acc = grx_vf[GRX_ACC].f;
    float r[4];
    r[0] = acc[0] - a[1] * b[2];
    r[1] = acc[1] - a[2] * b[0];
    r[2] = acc[2] - a[0] * b[1];
    r[3] = 0.0f;
    write_vf(d, 7, r);
}

void grx_vu0_mr32(int mask, int d, int s) {
    const float *a = grx_vf[s].f;
    float r[4];
    r[0] = a[1];
    r[1] = a[2];
    r[2] = a[3];
    r[3] = a[0];
    write_vf(d, mask, r);
}

void grx_vu0_move(int mask, int d, int s) {
    float r[4];
    memcpy(r, grx_vf[s].f, sizeof(r));
    write_vf(d, mask, r);
}

void grx_vu0_abs(int mask, int d, int s) {
    float r[4];
    int i;
    for (i = 0; i < 4; i++) {
        r[i] = grx_vf[s].f[i] < 0.0f ? -grx_vf[s].f[i] : grx_vf[s].f[i];
    }
    write_vf(d, mask, r);
}

void grx_vu0_itof(int mask, int d, int s, int frac) {
    float r[4];
    int i;
    for (i = 0; i < 4; i++) {
        r[i] = (float)grx_vf[s].i[i] / (float)(1 << frac);
    }
    write_vf(d, mask, r);
}

/* ftoi saturates, and writes integers into the register */
void grx_vu0_ftoi(int mask, int d, int s, int frac) {
    int i;
    if (d == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            double x = (double)grx_vf[s].f[i] * (double)(1 << frac);
            grx_vf[d].i[i] = x >= 2147483647.0 ? 0x7FFFFFFF : x <= -2147483648.0 ? (int)0x80000000 : (int)x;
        }
    }
}

void grx_vu0_named(const char *mnemonic, int d, int s, int t, int bc) {
    static const struct { const char *name; int op; } ops[] = {
        {"vadd", GRX_VADD}, {"vsub", GRX_VSUB}, {"vmul", GRX_VMUL}, {"vmadd", GRX_VMADD},
        {"vmsub", GRX_VMSUB}, {"vmax", GRX_VMAX}, {"vmini", GRX_VMINI},
    };
    const char *dot = strchr(mnemonic, '.');
    size_t len = dot ? (size_t)(dot - mnemonic) : strlen(mnemonic);
    int mask = 0;
    unsigned int k;
    if (dot) {
        for (dot++; *dot; dot++) {
            const char *f = strchr("xyzw", *dot);
            if (f) {
                mask |= 1 << (f - "xyzw");
            }
        }
    }
    for (k = 0; k < sizeof(ops) / sizeof(ops[0]); k++) {
        if (strlen(ops[k].name) == len && strncmp(ops[k].name, mnemonic, len) == 0) {
            grx_vu0_arith(ops[k].op, mask, d, s, t, bc);
            return;
        }
    }
    grx_fatal("grx_vu0_named: unsupported %s", mnemonic);
}

/* Save states (grx_state_parts.h): VU0's registers (the macro mode's) and
   the 128-bit GPR images; ee_misc.c's CRT setting */
extern int32_t grx_gs_crt_interlace, grx_gs_crt_mode, grx_gs_crt_ffmode;

void grx_ee_state_io(GrxState *s) {
    GRX_STATE_VAR(s, grx_vf);
    GRX_STATE_VAR(s, grx_vi);
    GRX_STATE_VAR(s, grx_gpr);
    GRX_STATE_VAR(s, grx_gs_crt_interlace);
    GRX_STATE_VAR(s, grx_gs_crt_mode);
    GRX_STATE_VAR(s, grx_gs_crt_ffmode);
}
