#include "common.h"

u32 *MovGsGifTag(u32 *p, u64 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3) {
    p[0] = (t2 << 15) | t3;
    p[1] = ((t1 << 14) | (a3 << 26)) | ((a2 << 28) | (t0 << 15));
    p[2] = a1 & 0xFFFFFFFF;
    p[3] = a1 >> 32;
    return p + 4;
}
