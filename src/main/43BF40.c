/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

#define COPY_MATRIX(dst, src) \
    __asm__ volatile( \
        "lqc2 $vf1, 0x0(%1)\n\t" \
        "lqc2 $vf2, 0x10(%1)\n\t" \
        "lqc2 $vf3, 0x20(%1)\n\t" \
        "lqc2 $vf4, 0x30(%1)\n\t" \
        "sqc2 $vf1, 0x0(%0)\n\t" \
        "sqc2 $vf2, 0x10(%0)\n\t" \
        "sqc2 $vf3, 0x20(%0)\n\t" \
        "sqc2 $vf4, 0x30(%0)" \
        : : "r"(dst), "r"(src) : "memory")

#define APPLY_MATRIX(out, m, in) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(out), "r"(m), "r"(in) : "memory")

typedef struct {
    u8 pad0[0x48];
    s32 count;
    u8 pad4C[0x14];
    FVECTOR *pos;
    u8 *nrm;
    u8 *uv;
} Obj;

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void VuVecNormalize(f32 *, f32 *);

void GphGaObjEnvMapCalcUvReflect(Obj *o, f32 (*mtx)[4]) {
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FMATRIX m;
    s32 i;
    s32 n;
    s16 (*uv)[2];
    s16 (*nrm)[4];
    FVECTOR *pos;
    f32 d, rx, ry, rz, len;

    uv = (s16 (*)[2])(o->uv + 0x10);
    nrm = (s16 (*)[4])(o->nrm + 0x10);
    pos = o->pos;
    n = o->count;
    COPY_MATRIX(m, mtx);
    VuVecNormalize(a, a);
    for (i = 0; i < n; i++) {
        b[0] = -pos[i][0];
        b[1] = -pos[i][1];
        b[2] = -pos[i][2];
        APPLY_MATRIX(b, m, b);
        c[0] = nrm[i][0] * (1.0f / 4096.0f);
        c[1] = nrm[i][1] * (1.0f / 4096.0f);
        c[2] = nrm[i][2] * (1.0f / 4096.0f);
        c[3] = 0.0f;
        APPLY_MATRIX(a, m, ((u16 *)nrm)[i]);
        VuVecNormalize(b, b);
        VuVecNormalize(a, a);
        d = b[0] * a[0] + b[1] * a[1] + b[2] * a[2];
        d = -d;
        d = d + d;
        rx = d * (a[0] - b[0]);
        ry = d * (a[1] - b[1]);
        rz = d * (a[2] - b[2]) + 1.0f;
        len = fsqrt(rx * rx + ry * ry + rz * rz);
        len = len + len;
        uv[i][0] = (rx / len + 0.5f) * 1024.0f;
        uv[i][1] = (ry / len + 0.5f) * 1024.0f;
    }
}
