/* Port-only C for GphGaObjDeformPulse (0x00438B78), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphGaObjDeformPulse(GphGaObject *o, f32 angle, f32 amp, f32 freq);

/* Pushes each vertex along its normal by
   amp * (sin(angle deg + x * freq) + sin(angle deg + z * freq * 0.2)),
   writing deformBuf0. Unskinned, the source is verts and the normals
   (4.12 fixed, 8 bytes each after a 0x10-byte header) are also handed on in
   deformBuf1; skinned, both are read from deformBuf0 itself (normals from
   its +0x10), as the asm does. */
void GphGaObjDeformPulse(GphGaObject *o, f32 angle, f32 amp, f32 freq) {
    GrxQuad *dst = o->deformBuf0;
    GrxQuad *src;
    s16 *nrm;
    u32 n;
    u32 i;
    f32 nx, ny, nz;
    f32 base, a, b, s;
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
    for (i = 0; i < n; i++) {
        /* vitof12 */
        nx = (f32)nrm[i * 4 + 0] * (1.0f / 4096.0f);
        ny = (f32)nrm[i * 4 + 1] * (1.0f / 4096.0f);
        nz = (f32)nrm[i * 4 + 2] * (1.0f / 4096.0f);
        __builtin_memcpy(&p, &src[i], 16);
        a = p.f[0] * freq;
        base = angle * 0.017453292f;
        b = p.f[2] * freq;
        a = base + a;
        b = b * 0.2f;
        s = VuSinf(a) * amp;
        s = s + VuSinf(base + b) * amp;
        p.f[0] = p.f[0] + nx * s;
        p.f[1] = p.f[1] + ny * s;
        p.f[2] = p.f[2] + nz * s;
        __builtin_memcpy(&dst[i], &p, 16);
    }
}
