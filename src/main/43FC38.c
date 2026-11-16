#include "common.h"
#include "main/n06.h"

void GphRippleAddRect(GphRipple *m, s32 x0, s32 y0, s32 w, s32 h, s32 val) {
    s32 x;
    s32 y;
    s32 mw;
    s32 mh;

    if (m == NULL) {
        return;
    }
    mw = m->w;
    mh = m->h;
    for (y = y0 + h; y >= y0; y--) {
        for (x = x0 + w; x >= x0; x--) {
            if (x < mw && x >= 0 && y < mh && y >= 0) {
                m->buf[m->cur][y * mw + x] += val;
            }
        }
    }
}
