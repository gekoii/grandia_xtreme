/* Port-only C for GphTexVertsProject (0x0043DBF8), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"
#include "vu0_port.h"

/* Scratchpad: screen scale and offset vectors, and the current view */
#define SPR_SCREEN_SCALE ((f32 *)0x70000060)
#define SPR_SCREEN_OFFSET ((f32 *)0x70000070)
#define SPR_CUR_VIEW (*(u8 **)0x700000EC)

void GphTexVertsProject(s32 n, void *out, f32 *in);

/*
 * Projects n textured vertices (two quadwords each: position, then s, t,
 * RGBA bits, unused) with the matrix-stack top into 0x18-byte GIF records:
 * ST = (s q, t q), RGBAQ = (rgba, q), XYZ2 = (x, y as u16 12.4, z >> 4),
 * q = (view + 0x18) / max(z, 1). An odd count gets 8 zero bytes after the
 * last record (to the next quadword).
 */
void GphTexVertsProject(s32 n, void *out, f32 *in) {
    u8 *o = out;
    f32 *top = (f32 *)VuMtxStackGetTop();
    f32 *scale = SPR_SCREEN_SCALE;
    f32 *ofs = SPR_SCREEN_OFFSET;
    f32 qnum = *(f32 *)(SPR_CUR_VIEW + 0x18);
    u32 i;

    for (i = 0; i < (u32)n; i++) {
        f32 v[4];
        f32 st[4];
        f32 q;
        f32 x;
        f32 y;
        f32 z;
        s32 ix;
        s32 iy;
        s32 iz;

        VuCopy4(v, in);
        VuCopy4(st, in + 4);
        v[3] = 1.0f;
        VuApplyMtx(v, v, top);
        if (v[2] < 1.0f) {
            v[2] = 1.0f;
        }
        q = VuDiv(qnum, v[2]);
        x = ofs[0] + v[0] * q * scale[0];
        y = ofs[1] + v[1] * q * scale[1];
        z = ofs[2] + 1.0f * q * scale[2];
        if (x > VuBits(0x477FF000)) { /* 65520 */
            x = VuBits(0x477FF000);
        }
        if (y > VuBits(0x477FF000)) {
            y = VuBits(0x477FF000);
        }
        if (x < 0.0f) {
            x = 0.0f;
        }
        if (y < 0.0f) {
            y = 0.0f;
        }
        ix = VuFtoi(x, 0);
        iy = VuFtoi(y, 0);
        iz = VuFtoi(z, 0);
        st[0] = st[0] * q;
        st[1] = st[1] * q;
        __builtin_memcpy(o + 0x0, &st[0], 8);
        __builtin_memcpy(o + 0x8, &st[2], 4);
        __builtin_memcpy(o + 0xC, &q, 4);
        *(u32 *)(o + 0x10) = (u16)ix | ((u32)(u16)iy << 16);
        *(u32 *)(o + 0x14) = (u32)iz >> 4;
        o += 0x18;
        in += 8;
    }
    if ((u32)o & 0xF) {
        *(u32 *)o = 0;
        *(u32 *)(o + 4) = 0;
    }
}
