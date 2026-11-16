#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
} Rect;

void *memcpy(u8 *, u8 *, s32);

void GsRectCopy(Rect *dr, s32 dpitch, u8 *dbuf, Rect *sr, s32 spitch, u8 *sbuf, s32 bpp) {
    u8 *d;
    u8 *s;
    s32 i;
    s32 bytes;

    if (sr->h <= 0) {
        return;
    }
    s = sbuf + spitch * sr->y + sr->x;
    d = dbuf + dpitch * dr->y + dr->x;
    bytes = bpp >> 3;
    if (d < s) {
        for (i = 0; i < sr->h; i++) {
            memcpy(d, s, sr->w * bytes);
            s += spitch;
            d += dpitch;
        }
    } else {
        s += spitch * (sr->h - 1);
        d += dpitch * (sr->h - 1);
        for (i = 0; i < sr->h; i++) {
            memcpy(d, s, sr->w * bytes);
            s -= spitch;
            d -= dpitch;
        }
    }
}
