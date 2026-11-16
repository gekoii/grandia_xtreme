/*
 * The R5900 instructions the game's inline asm uses, emulated in C.
 *
 * port/tools/asm2c.py rewrites each asm statement into calls on these. The
 * VU0 registers, ACC and the GPRs that templates name ($2, $v0, ...) are
 * global, like the real registers, so values handed from one asm statement
 * to the next survive. The port runs the game's threads cooperatively, so
 * one register file is enough.
 *
 * No system headers: this is included first in every game file, and the
 * game declares its own libc prototypes.
 */
#ifndef GRX_EE_H
#define GRX_EE_H

typedef union {
    float f[4];
    unsigned int u[4];
    int i[4];
    unsigned long long d[2];
    unsigned char b[16];
} GrxQuad;

/* $vf0-$vf31, then ACC */
#define GRX_ACC 32
extern GrxQuad grx_vf[33];
extern unsigned int grx_vi[32];
extern GrxQuad grx_gpr[32];

#define GRX_UPTR __UINTPTR_TYPE__
#define GRX_ADDR(base, off) ((void *)((unsigned char *)(GRX_UPTR)(base) + (off)))

/* Memory ordering: the port is single-threaded */
#define grx_sync() __asm__ volatile("" ::: "memory")

/* lq/sq. The R5900 ignores the low 4 address bits; the port doesn't, since
   its data needn't be 16-aligned where the game's was. */
#define grx_lq(reg, addr) __builtin_memcpy((reg), (addr), 16)
#define grx_sq(reg, addr) __builtin_memcpy((addr), (reg), 16)

void grx_lqc2(int vf, const void *addr);
void grx_sqc2(int vf, void *addr);

/* qmtc2/qmfc2 between a VF register and a C value of any size (a float or
   int lands in x, a 128-bit value fills the register) */
void grx_qmtc2_raw(int vf, const void *src, unsigned int size);
void grx_qmfc2_raw(void *dst, int vf, unsigned int size);
#define GRX_QMTC2(vf, x) do { __typeof__(x) _v = (x); grx_qmtc2_raw((vf), &_v, sizeof(_v)); } while (0)
#define GRX_QMFC2(lv, vf) grx_qmfc2_raw(&(lv), (vf), sizeof(lv))
#define grx_qmtc2_q(vf, reg) grx_qmtc2_raw((vf), (reg), 16)
#define grx_qmfc2_q(reg, vf) grx_qmfc2_raw((reg), (vf), 16)

unsigned int grx_cfc2(int vi);
void grx_ctc2(int vi, unsigned int value);

/* FPU */
#define grx_sqrt_s(x) __builtin_sqrtf(x)
/* div.s as the EE does it: no infinities or NaNs, so x/0 (0/0 too, and
   a denormal divisor, which the EE reads as 0) is +-FLT_MAX with the
   operands' sign, and an overflow clamps to it. For the game's C where a
   host division would make an infinity the EE never did
   (port/tools/fixups/eefloat.py). */
static inline float grx_div_s(float a, float b) {
    union {
        float f;
        unsigned int u;
    } x, y, r;
    x.f = a;
    y.f = b;
    if ((y.u & 0x7F800000u) == 0) {
        r.u = ((x.u ^ y.u) & 0x80000000u) | 0x7F7FFFFFu;
        return r.f;
    }
    r.f = a / b;
    if ((r.u & 0x7F800000u) == 0x7F800000u) {
        r.u = (r.u & 0x80000000u) | 0x7F7FFFFFu;
    }
    return r.f;
}
#define grx_min_s(a, b) ((a) < (b) ? (a) : (b))
#define grx_max_s(a, b) ((a) > (b) ? (a) : (b))
/* trunc.w.s leaves an integer in a float register; mfc1/mtc1 move bits */
#define GRX_TRUNC_W_S(fd, fs) do { int _i = (int)(fs); __builtin_memcpy(&(fd), &_i, 4); } while (0)
#define GRX_MFC1(rt, fs) do { unsigned int _b; __builtin_memcpy(&_b, &(fs), 4); (rt) = _b; } while (0)
#define GRX_MTC1(fs, rt) do { unsigned int _b = (unsigned int)(rt); __builtin_memcpy(&(fs), &_b, 4); } while (0)

/* VU0 macro mode. Masks: bit 0 = x ... bit 3 = w. bc: the broadcast field
   of the third operand, or -1. */
enum { GRX_VADD, GRX_VSUB, GRX_VMUL, GRX_VMADD, GRX_VMSUB, GRX_VMAX, GRX_VMINI };
void grx_vu0_arith(int op, int mask, int d, int s, int t, int bc);
void grx_vu0_opmula(int s, int t);
void grx_vu0_opmsub(int d, int s, int t);
void grx_vu0_mr32(int mask, int d, int s);
void grx_vu0_move(int mask, int d, int s);
void grx_vu0_abs(int mask, int d, int s);
void grx_vu0_itof(int mask, int d, int s, int frac);
void grx_vu0_ftoi(int mask, int d, int s, int frac);
/* A mnemonic given as a string ("vadd.xyz"), for templates pasted together
   in macros */
void grx_vu0_named(const char *mnemonic, int d, int s, int t, int bc);

/* vcallms / vcallmsr: the VU0 microprograms (port/src/vu0_micro.c) */
#include "grx_vu0_micro.h"

#endif
