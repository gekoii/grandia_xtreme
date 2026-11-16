#include "common.h"
#include "protos/main.h"


u32 *MovGsClamp(u32 *p, u32 a1, u32 a2, u32 a3, u32 t0, u32 t1, u32 t2) {
    return MovGsAd(p, 8, (u64)a1 | ((u64)a2 << 2) | ((u64)a3 << 4) | ((u64)t0 << 14) | ((u64)t1 << 24) | ((u64)t2 << 34));
}
