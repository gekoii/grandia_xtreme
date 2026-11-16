#include "common.h"
#include "protos/main.h"


u32 *MovGsTex0(u32 *arg0, u32 tbp, u32 tbw, u32 psm, u32 tw, u32 th, u32 tcc, u32 tfx, u32 cbp, u32 cpsm, u32 csm, u32 csa, u32 cld) {
    return MovGsAd(arg0, 6,
                         (u64)tbp | ((u64)tbw << 14) | ((u64)psm << 20) | ((u64)tw << 26) | ((u64)th << 30) |
                             ((u64)tcc << 34) | ((u64)tfx << 35) | ((u64)cbp << 37) | ((u64)cpsm << 51) |
                             ((u64)csm << 55) | ((u64)csa << 56) | ((u64)cld << 61));
}
