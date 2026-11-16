#include "common.h"
#include "protos/main.h"


u32 *MovGsTest(u32 *arg0, u32 ate, u32 atst, u32 aref, u32 afail, u32 date, u32 datm, u32 zte, u32 ztst) {
    return MovGsAd(arg0, 0x47,
                  (u64)ate | ((u64)atst << 1) | ((u64)aref << 4) | ((u64)afail << 12) |
                  ((u64)date << 14) | ((u64)datm << 15) | ((u64)zte << 16) | ((u64)ztst << 17));
}
