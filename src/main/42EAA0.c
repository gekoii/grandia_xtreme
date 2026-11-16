#include "common.h"

void FileLzssDecode(u8 *src, u8 *dst, s32 len) {
    u8 text[0x800 + 34 - 1];
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    s32 c;
    u32 flags;
    s32 in;
    s32 out;

    for (i = 0; i < 0x7DE; i++) {
        text[i] = 0;
    }
    r = 0x7DE;
    out = 0;
    in = 0;
    flags = 0;
    for (;;) {
        if (!((flags >>= 1) & 0x100)) {
            if (in == len) {
                break;
            }
            c = src[in++];
            flags = c | 0xFF00;
        }
        if (flags & 1) {
            if (in == len) {
                break;
            }
            c = src[in++];
            text[r++] = dst[out++] = c;
            r &= 0x7FF;
        } else {
            if (in == len) {
                break;
            }
            i = src[in++];
            if (in == len) {
                break;
            }
            j = src[in++];
            i |= (j & 0xE0) << 3;
            j = (j & 0x1F) + 2;
            for (k = 0; k <= j; k++) {
                c = text[(i + k) & 0x7FF];
                text[r++] = dst[out++] = c;
                r &= 0x7FF;
            }
        }
    }
}
