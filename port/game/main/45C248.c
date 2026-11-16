/* Port-only C for VuProjectVerts (0x0045C248), which the decompilation still has in assembly. */
/*
 * The asm runs VU0 microprograms 0x100, 0x140 and 0x1C0 (vuMicroCode0,
 * assets/main/095A60.textbin.bin) as a software pipeline: each transforms
 * the next vertex by mtx ($vf4-$vf7), starts the divide for its q and
 * finishes the previous one. Disassembled, every output vertex is
 *     (X, Y, Z) = (v.xyz, 1) * mtx
 *     q = SPR80.w / Z
 *     out = (o.x + s.x X q, o.y + s.y Y q, o.z + SPR60.z q, 1)
 * with s.xy = SPR60.xy / 16, o.xy = SPR70.xy / 16 - 2048 and
 * o.z = SPR70.z / 16 (the EE's setup before the first vcallms), the
 * scratchpad's screen scale/offset at 0x70000060/0x70000070 and the
 * projection numerator at 0x70000080.
 *
 * Vertices go in pairs: n is rounded up to even ((n + 1) & 0xFFFE, as the
 * asm masks it). n = 0 makes the asm's loop run away; the port writes
 * nothing then.
 *
 * VU0 registers: the PS2 leaves $vf1, $vf2, $vf4-$vf18, $vi1 and Q
 * changed; no caller reads them afterwards (C callers 40C090.c, 5D50C0.c
 * and the asm callers load their own), so the port doesn't write grx_vf.
 */
#include "common.h"
#include "vu0_port.h"

#define SPR_SCREEN_SCALE ((f32 *)0x70000060)
#define SPR_SCREEN_OFFSET ((f32 *)0x70000070)
#define SPR_PROJ ((f32 *)0x70000080)

void VuProjectVerts(f32 *out, f32 *mtx, f32 *verts, s32 n);

void VuProjectVerts(f32 *out, f32 *mtx, f32 *verts, s32 n) {
    f32 s[4];
    f32 o[4];
    f32 num;
    s32 count;
    s32 i;

    VuCopy4(s, SPR_SCREEN_SCALE);
    VuCopy4(o, SPR_SCREEN_OFFSET);
    num = SPR_PROJ[3];
    s[0] = s[0] * VuBits(0x3D800000); /* 1/16 */
    s[1] = s[1] * VuBits(0x3D800000);
    o[0] = -VuBits(0x45000000) + o[0] * VuBits(0x3D800000); /* 2048 */
    o[1] = -VuBits(0x45000000) + o[1] * VuBits(0x3D800000);
    o[2] = 0.0f + o[2] * VuBits(0x3D800000);

    count = (n + 1) & 0xFFFE;
    for (i = 0; i < count; i++) {
        f32 v[4];
        f32 r[4];
        f32 q;

        VuCopy4(v, &verts[i * 4]);
        v[3] = 1.0f;
        VuApplyMtx(v, v, mtx);
        q = VuDiv(num, v[2]);
        r[0] = o[0] + (s[0] * v[0]) * q;
        r[1] = o[1] + (s[1] * v[1]) * q;
        r[2] = o[2] + s[2] * q;
        r[3] = 1.0f;
        VuCopy4(&out[i * 4], r);
    }
}
