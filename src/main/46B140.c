#include "common.h"
#include "protos/main.h"


u32 *MovGsBitBltBuf(u32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    return (s32)MovGsAd(arg0, 0x50, (u64)arg3 << 56 | (u64)arg2 << 48 | (u64)arg1 << 32);
}
