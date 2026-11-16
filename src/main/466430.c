#include "common.h"
#include "m2c_macros.h"

void scTag2(s64 *arg0, u32 arg1, s32 arg2, s32 arg3) {
    *arg0 = ((u64)arg1 << 0x20) | ((u64)((s64)arg2 << 0x20) >> 4) | (u32)arg3;
}
