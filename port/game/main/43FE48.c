/* Port-only C for GphRippleApply8 (0x0043FE48), which the decompilation still has in assembly. */
#include "common.h"
#include "main/n06.h"

void GphRippleApply8(GphRipple *r);

/* The 8-bit version of GphRippleApply32: each output byte is the source
   texel displaced by the height field's slope there (wrapping), four per
   output word. The slopes read one past each row and one row below the
   last, as the asm does (the field has margin rows). */
#define PIX(k) tex[(u32)(src[(k) + 1] - src[k] + (k)) % w + \
                   (u32)(src[(k) + w] - src[k] + y) % h * w]

void GphRippleApply8(GphRipple *r) {
    u8 *tex;
    u32 *out;
    s32 *src;
    u32 w;
    u32 h;
    u32 x;
    s32 y;
    u32 c0, c1, c2, c3;

    if (r == NULL) {
        return;
    }
    tex = (u8 *)r->src;
    out = (u32 *)r->dst;
    src = r->buf[r->cur];
    w = r->w;
    h = r->h;
    for (y = 0; y < (s32)h; y++) {
        x = 0;
        do {
            c0 = PIX(x);
            c1 = PIX(x + 1);
            c2 = PIX(x + 2);
            c3 = PIX(x + 3);
            *out++ = (c3 << 24) | c0 | (c1 << 8) | ((c2 << 16) & 0xFF0000);
            x += 4;
        } while (x != w);
        src += w;
    }
}
