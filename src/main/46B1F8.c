#include "common.h"
#include "protos/main.h"


u32 *MovGsAlpha(u32 *a0, u32 a1, u32 a2, u32 a3, u32 t0, u32 t1) {
    return MovGsAd(a0, 0x42, ((u64)t1 << 32) | ((u64)t0 << 6) | ((u64)a3 << 4) | ((u64)a2 << 2) | (u64)a1);
}
