#include "common.h"
#include "protos/main.h"


u32 *MovGsTrxPos(u32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    return MovGsAd(arg0, 0x51, ((u64)arg1 << 59) | ((u64)arg3 << 48) | ((u64)arg2 << 32));
}
