#include "common.h"

void GphClutToSepia(u8 *dst, u8 *src) {
    s32 i;
    f32 g;

    for (i = 0; i < 256; i++) {
        g = (f32)((src[0] + src[1] + src[2]) / 3) * 0.00390625f;
        dst[0] = g * 240.0f;
        dst[1] = g * 176.0f;
        dst[2] = g * 140.0f;
        dst[3] = src[3];
        src += 4;
        dst += 4;
    }
}
