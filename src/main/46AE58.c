#include "common.h"
#include "protos/main.h"
u32 *MovGsUv(u32 *arg0, u32 arg1, u32 arg2) {
    return MovGsAd(arg0, 3, (u64)arg1 | ((u64)arg2 << 16));
}
