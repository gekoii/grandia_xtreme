#include "common.h"

u32 McAdler32(u32 adler, u8 *buf, u32 len) {
    u32 s1 = adler & 0xFFFF;
    u32 s2 = adler >> 16;
    u32 i;

    for (i = 0; i < len; i++) {
        s1 = (s1 + buf[i]) % 65521;
        s2 = (s2 + s1) % 65521;
    }
    return (s2 << 16) | s1;
}
