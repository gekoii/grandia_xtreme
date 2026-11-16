#include "common.h"
#include "protos/main.h"


u32 *MovGsXyz2(u32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    return MovGsAd(arg0, 5, (u64)arg1 | ((u64)arg2 << 16) | ((u64)arg3 << 32));
}
