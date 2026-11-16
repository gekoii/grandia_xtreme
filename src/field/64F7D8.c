#include "common.h"

u32 MiniAlignUp(u32 n, u32 align) {
    u32 r;
    u32 m;
    u32 *p;

    r = n;
    m = r % align;
    if (m != 0) {
        /* the remainder goes through a pointer to r: plain r = align - m is 8 words off */
        p = &r;
        *p = align - m;
        r = n + r;
    }
    return r;
}
