#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


GphRipple *GphRippleCreate(u16 w, u16 h, s32 *src, s32 *dst, GphRipple *s) {
    s32 *p;
    s32 size;
    s32 i;

    p = s->data;
    size = GphRippleWorkSize(w, h);
    for (i = 0; i < size / 4; i++) {
        ((s32 *)s)[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        s->bufBase[i] = p;
        p += w * (h + 4);
    }
    for (i = 0; i < 2; i++) {
        s->buf[i] = s->bufBase[i] + w * 2;
    }
    s->w = w;
    s->h = h;
    s->src = src;
    s->dst = dst;
    s->cur = 0;
    return s;
}
