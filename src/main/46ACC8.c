#include "common.h"
#include "protos/main.h"


u32 *MovGsTex1(u32 *arg0, u32 lcm, u32 mxl, u32 mmag, u32 mmin, u32 mtba, u32 l, u32 k) {
    return MovGsAd(arg0, 0x14,
                         (u64)lcm | ((u64)mxl << 2) | ((u64)mmag << 5) | ((u64)mmin << 6) |
                             ((u64)mtba << 9) | ((u64)l << 19) | ((u64)k << 32));
}
