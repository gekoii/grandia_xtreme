#include "common.h"
#include "protos/main.h"
u32 *MovGsFrame(u32 *arg0, u32 a1, u32 a2, u32 a3, u32 t0) {
    return MovGsAd(arg0, 0x4C, (u64)a1 | ((u64)a2 << 16) | ((u64)a3 << 24) | ((u64)t0 << 32));
}
