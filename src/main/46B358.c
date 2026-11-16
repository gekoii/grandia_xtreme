#include "common.h"
#include "protos/main.h"


u32 *MovGsZbuf(u32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    return MovGsAd(arg0, 0x4E, ((u64)arg1 << 32) | ((u64)arg2 << 24) | (u64)arg3);
}
