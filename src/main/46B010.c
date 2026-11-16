#include "common.h"
#include "protos/main.h"


u32 *MovGsXyOffset(u32 *arg0, u32 arg1, s32 arg2) {
    return MovGsAd(arg0, 0x18, arg1 | ((u64)arg2 << 32));
}
