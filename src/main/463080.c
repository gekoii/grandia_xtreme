#include "common.h"

s8 *SndIniReadLine(s8 *dst, s8 *src) {
    s32 active = 1;
    s8 c;

    while (*src != '\n') {
        c = *src;
        if (c <= ' ') {
            c = 0;
        }
        src++;
        if (c == ';') {
            c = 0;
            active = 0;
        }
        if (c != 0 && active != 0) {
            *dst++ = c;
        }
    }
    *dst = 0;
    return src + 1;
}
