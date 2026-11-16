#include "common.h"

u64 *MovGsDmaTag(u64 *p, u32 a1, u32 a2, u32 a3, u32 t0, u32 t1, u32 t2) {
    p[0] = ((u64)a1 << 63) | ((u64)(a2 & 0xFFFFFFF0) << 32) | ((u64)a3 << 31) | ((u64)t0 << 28) | ((u64)t1 << 26) | (u64)t2;
    return p + 2;
}
