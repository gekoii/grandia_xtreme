#include "common.h"

u32 *MovGsAd(u32 *p, u32 a1, u64 a2) {
    p[0] = a2 & 0xFFFFFFFF;
    p[1] = a2 >> 32;
    p[2] = a1;
    p[3] = 0;
    return p + 4;
}
