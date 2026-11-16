/* Port-only C for GphGaObjDeformCollapse (0x00438E40), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"

void GphGaObjDeformCollapse(GphGaObject *o, f32 t, f32 k, f32 unused);

/* div.s on the EE: dividing by zero gives the largest float, signed as
   the operands' signs XORed, where IEEE gives an infinity or a NaN */
static f32 EeDiv(f32 a, f32 b) {
    union {
        f32 f;
        u32 u;
    } x, y, r;

    if (b == 0.0f) {
        x.f = a;
        y.f = b;
        r.u = ((x.u ^ y.u) & 0x80000000) | 0x7F7FFFFF;
        return r.f;
    }
    return a / b;
}

static f32 Clamp50(f32 v) {
    if (50.0f < v) {
        v = 50.0f;
    }
    if (v < -50.0f) {
        v = -50.0f;
    }
    return v;
}

/* Drops each vertex by t * t * k (stopping at y 0) and spreads it outwards
   along its normal's horizontal direction by a tenth of that, at most 50
   each way, writing deformBuf0. Sources as GphGaObjDeformPulse: verts and
   the 4.12 normals (deformBuf1 is set to them), or deformBuf0 itself when
   skinned. The fourth argument ($f14) isn't read. */
void GphGaObjDeformCollapse(GphGaObject *o, f32 t, f32 k, f32 unused) {
    GrxQuad *dst = o->deformBuf0;
    GrxQuad *src;
    s16 *nrm;
    u32 n;
    u32 i;
    f32 nx, nz, len, drop, y, d;
    GrxQuad p;

    if (dst == NULL) {
        return;
    }
    n = o->vtxCount;
    if (o->skin == NULL) {
        src = (GrxQuad *)o->verts;
        o->deformBuf1 = o->normals;
        nrm = (s16 *)((u8 *)o->normals + 0x10);
    } else {
        src = dst;
        nrm = (s16 *)((u8 *)dst + 0x10);
    }
    if (n == 0) {
        return;
    }
    t = t * t;
    for (i = 0; i < n; i++) {
        /* vitof12 */
        nx = (f32)nrm[i * 4 + 0] * (1.0f / 4096.0f);
        nz = (f32)nrm[i * 4 + 2] * (1.0f / 4096.0f);
        __builtin_memcpy(&p, &src[i], 16);
        drop = t * k;
        y = p.f[1] - drop;
        p.f[1] = y;
        len = __builtin_sqrtf(nx * nx + nz * nz);
        nx = EeDiv(nx, len);
        nz = EeDiv(nz, len);
        d = Clamp50(drop * nx * 0.1f);
        p.f[0] = p.f[0] + d;
        d = Clamp50(drop * nz * 0.1f);
        p.f[2] = p.f[2] + d;
        if (y < 0.0f) {
            p.f[1] = 0.0f;
        }
        __builtin_memcpy(&dst[i], &p, 16);
    }
}
