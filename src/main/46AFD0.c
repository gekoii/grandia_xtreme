#include "common.h"
#include "protos/main.h"


u32 *MovGsScissor(u32 *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    return MovGsAd(arg0, 0x40, (u64)arg1 | ((u64)arg2 << 16) | ((u64)arg3 << 32) | ((u64)arg4 << 48));
}
