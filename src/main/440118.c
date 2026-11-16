#include "common.h"
#include "main/n06.h"

void GphRippleApply32(GphRipple *arg0, s32 arg1) {
    s32 y;
    s32 x;
    s32 w;
    s32 h;
    s32 *out;
    s32 *t;
    s32 *data;
    u32 row;
    u32 col;

    if (arg0 != NULL) {
        h = arg0->h;
        data = arg0->src;
        out = arg0->dst;
        w = arg0->w;
        t = arg0->buf[arg0->cur];
        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                row = t[x + w] - t[x] + y;
                col = t[x + 1] - t[x] + x;
                col %= (u32)w;
                row %= (u32)h;
                out[x] = data[col + row * w];
            }
            out += arg1;
            t += w;
        }
    }
}
