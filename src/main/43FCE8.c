#include "common.h"
#include "main/n06.h"

void GphRippleUpdate(GphRipple *r, u8 damp) {
    s32 *src;
    s32 *dst;
    s32 w;
    u32 y;
    s32 x;
    s32 v;

    if (r != NULL) {
        src = r->buf[r->cur];
        dst = r->buf[r->cur ^ 1];
        w = r->w;
        for (y = 0; y < r->h; y++) {
            for (x = 0; x < w; x++) {
                v = (src[x + 1] + src[w + x + 1] + src[x + 1 - w] + src[x - 1] + src[w + x - 1] + src[x - 1 - w] + src[w + x] + src[x - w]) >> 2;
                v -= dst[x];
                dst[x] = v - (v >> damp);
            }
            src += w;
            dst += w;
        }
        r->cur ^= 1;
    }
}
