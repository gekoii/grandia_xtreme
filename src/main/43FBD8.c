#include "common.h"
#include "main/n06.h"
void GphRippleAddPoint(GphRipple *g, s32 x, s32 y, s32 v) {
    s32 w, h;
    if (g != NULL) {
        w = g->w;
        h = g->h;
        if (x < w && x >= 0 && y < h && y >= 0) {
            g->buf[g->cur][y * w + x] += v;
        }
    }
}
