/* ABI functions the interpreter implements itself (port/include/grx_mips.h).

   The soft-float double helpers of the main executable's libgcc (fp-bit:
   dpadd = __adddf3, ...), which the port doesn't have under these names:
   on the EE a double is a 64-bit value in one GPR, passed and returned
   like a 64-bit integer, while floats travel in $f12/$f0. The host's
   doubles are the same IEEE format.

   And two VU0 helpers that are still assembly in the port (stubs there):
   VuBezierInterp (asm/main/main/03E060.s) and VuVecNormalizeRcpLen
   (asm/main/main/05DA20.s), done here on the port's VU0 state the way the
   assembly does them. */
#include <math.h>
#include <string.h>
#include "grx_ee.h"
#include "grx_mips.h"

void grx_mips_bi_dpadd(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_d(c, grx_mips_arg_d(c, 0) + grx_mips_arg_d(c, 1));
}

void grx_mips_bi_dpsub(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_d(c, grx_mips_arg_d(c, 0) - grx_mips_arg_d(c, 1));
}

void grx_mips_bi_dpmul(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_d(c, grx_mips_arg_d(c, 0) * grx_mips_arg_d(c, 1));
}

void grx_mips_bi_litodp(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_d(c, (double)grx_mips_arg_i(c, 0));
}

/* __fixdfsi: truncates; out of range saturates (NaN gives the maximum) */
void grx_mips_bi_dptoli(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    double d = grx_mips_arg_d(c, 0);
    (void)f;
    if (!(d < 2147483648.0)) {
        grx_mips_ret_i(c, 0x7FFFFFFF);
    } else if (d <= -2147483648.0) {
        grx_mips_ret_i(c, (int32_t)0x80000000u);
    } else {
        grx_mips_ret_i(c, (int32_t)d);
    }
}

void grx_mips_bi_dptofp(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_f(c, (float)grx_mips_arg_d(c, 0));
}

void grx_mips_bi_fptodp(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    (void)f;
    grx_mips_ret_d(c, (double)grx_mips_arg_f(c, 0));
}

/* dst = the point at t on the cubic Bezier curve of ctrl[0..3] (vectors of
   4 floats), by de Casteljau, as the assembly does it in vf1-vf6 */
void grx_mips_bi_VuBezierInterp(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    float *dst = grx_mips_arg_p(c, 0);
    const float *ctrl = grx_mips_arg_p(c, 1);
    float t = grx_mips_arg_f(c, 0), u = 1.0f - t;
    float p[4][4];
    int i, k, n;
    (void)f;
    memcpy(p, ctrl, sizeof(p));
    for (n = 3; n > 0; n--) {
        for (k = 0; k < n; k++) {
            for (i = 0; i < 4; i++) {
                p[k][i] = p[k][i] * u + p[k + 1][i] * t;
            }
        }
    }
    memcpy(dst, p[0], 16);
    /* the registers it leaves (vf1-vf4 are the last values; vf6 = t, 1 - t) */
    memcpy(&grx_vf[1], p[0], 16);
    grx_vf[6].f[0] = t;
    grx_vf[6].f[3] = u;
}

/* dst.xyz = src.xyz / |src.xyz|, dst.w = src.w; returns 1 / |src.xyz| */
void grx_mips_bi_VuVecNormalizeRcpLen(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    float *dst = grx_mips_arg_p(c, 0);
    const float *src = grx_mips_arg_p(c, 1);
    float v[4], l2, q;
    (void)f;
    memcpy(v, src, 16);
    l2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    q = l2 > 0.0f ? 1.0f / sqrtf(l2) : 3.402823466e38f;
    v[0] *= q;
    v[1] *= q;
    v[2] *= q;
    memcpy(dst, v, 16);
    memcpy(&grx_vf[1], v, 16);
    grx_mips_ret_f(c, q);
}

GrxMipsHandler grx_mips_builtin(const char *name) {
    static const struct {
        const char *name;
        GrxMipsHandler h;
    } tab[] = {
        {"dpadd", grx_mips_bi_dpadd},   {"dpsub", grx_mips_bi_dpsub},   {"dpmul", grx_mips_bi_dpmul},
        {"litodp", grx_mips_bi_litodp}, {"dptoli", grx_mips_bi_dptoli}, {"dptofp", grx_mips_bi_dptofp},
        {"fptodp", grx_mips_bi_fptodp}, {"VuBezierInterp", grx_mips_bi_VuBezierInterp},
        {"VuVecNormalizeRcpLen", grx_mips_bi_VuVecNormalizeRcpLen},
    };
    unsigned i;
    for (i = 0; i < sizeof(tab) / sizeof(tab[0]); i++) {
        if (strcmp(tab[i].name, name) == 0) {
            return tab[i].h;
        }
    }
    return 0;
}
