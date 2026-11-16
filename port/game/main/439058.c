/* Port-only C for GphGaObjDeformWaveLit (0x00439058), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

/* The KSEG aliases the asm goes through: uncached for the stores,
 * uncached-accelerated for the loads (same memory as the plain address) */
#define UNCACHED(p) ((u8 *)((u32)(p) | 0x20000000))
#define UNCACHED_ACC(p) ((u8 *)((u32)(p) | 0x30000000))

void VuVecNormalize(f32 *dst, f32 *src);

void GphGaObjDeformWaveLit(GphGaObject *obj, f32 angle, f32 amp, f32 freq);

/*
 * The GphGaObjDeformWave (4386A0) wave from the original vertices, written
 * to the vertex packet, plus normals from finite differences 10 units along
 * x and z (normalised, packed 4.12 after a copy of the original normal
 * packet's header and contents) and a copy of the UV packet.
 *
 * Kept as the asm has it: y moves by a + z * freq (z * freq, not the
 * second sine), and z by the second sine * 0.2 * 0.2.
 *
 * The asm builds the moved vertex in a stack quadword whose w it never
 * writes (stack garbage on the PS2); the port keeps the vertex's own w.
 */
void GphGaObjDeformWaveLit(GphGaObject *obj, f32 angle, f32 amp, f32 freq) {
    u8 *pkt = obj->vtxPkt;
    u8 *nrmBuf;
    u8 *uvBuf;
    f32 *verts;
    s16 *nrmOut;
    u32 n;
    u32 i;
    u32 cnt;
    f32 base;

    if (pkt == NULL) {
        return;
    }
    nrmBuf = obj->deformBuf0;
    if (nrmBuf == NULL) {
        return;
    }
    uvBuf = obj->deformBuf1;
    if (uvBuf == NULL) {
        return;
    }
    n = obj->vtxCount;
    obj->flags |= 2;
    verts = (f32 *)obj->verts;

    /* Normal packet: 16-byte header + 8 bytes per vertex */
    cnt = (n + 3) >> 1;
    for (i = 0; i < cnt; i++) {
        VuCopy4(UNCACHED(nrmBuf) + i * 16, UNCACHED_ACC(obj->normals) + i * 16);
    }
    nrmOut = (s16 *)(nrmBuf + 0x10);
    /* UV packet: 16-byte header + 4 bytes per vertex */
    cnt = (n + 7) >> 2;
    for (i = 0; i < cnt; i++) {
        VuCopy4(UNCACHED(uvBuf) + i * 16, UNCACHED_ACC(obj->uvs) + i * 16);
    }

    base = angle * VuBits(0x3C8EFA35); /* pi / 180 */
    for (i = 0; i < n; i++) {
        f32 p[4];
        f32 o[4];
        f32 dx[4];
        f32 dz[4];
        f32 nv[4];
        s32 w[4];
        f32 fx;
        f32 fz;
        f32 sa;
        f32 sb;
        f32 sc;
        f32 sd;

        VuCopy4(p, &verts[i * 4]);
        fx = p[0] * freq;
        fz = p[2] * freq;
        sa = VuSinf(base + fx) * amp;
        sb = VuSinf(base + fz) * amp;
        o[0] = p[0] + sa * VuBits(0x3E4CCCCD); /* 0.2 */
        o[1] = p[1] + (sa + fz);
        sb = sb * VuBits(0x3E4CCCCD);
        o[2] = p[2] + sb * VuBits(0x3E4CCCCD);
        o[3] = p[3];
        sc = VuSinf(base + (p[0] + 10.0f) * freq) * amp;
        sd = VuSinf(base + (p[2] + 10.0f) * freq) * amp;

        /* d/dx and d/dz of the surface; normal = dz x dx */
        dx[0] = 10.0f;
        dx[1] = sc - sa;
        dx[2] = 0.0f;
        dx[3] = 0.0f;
        dz[0] = 0.0f;
        dz[1] = sd * VuBits(0x3E4CCCCD) - sb;
        dz[2] = 10.0f;
        dz[3] = 0.0f;
        nv[0] = dz[1] * dx[2] - dx[1] * dz[2];
        nv[1] = dz[2] * dx[0] - dx[2] * dz[0];
        nv[2] = dz[0] * dx[1] - dx[0] * dz[1];
        nv[3] = dz[3];
        VuVecNormalize(nv, nv);

        VuCopy4(pkt + i * 16, o);
        w[0] = VuFtoi(nv[0], 12);
        w[1] = VuFtoi(nv[1], 12);
        w[2] = VuFtoi(nv[2], 12);
        w[3] = VuFtoi(nv[3], 12);
        VuPackH4(&nrmOut[i * 4], w);
    }
}
