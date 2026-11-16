#include "common.h"
#include "protos/main.h"


u32 *MovGsTrxReg(u32 *arg0, u32 arg1, u32 arg2) {
    return MovGsAd(arg0, 0x52, ((u64)arg2 << 32) | arg1);
}
