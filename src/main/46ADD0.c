#include "common.h"
#include "protos/main.h"


u32 *MovGsPrim(u32 *arg0, u32 prim, u32 iip, u32 tme, u32 fge, u32 abe, u32 aa1, u32 fst, u32 ctxt, u32 fix) {
    return MovGsAd(arg0, 0,
                         (u64)prim | ((u64)iip << 3) | ((u64)tme << 4) | ((u64)fge << 5) | ((u64)abe << 6) |
                             ((u64)aa1 << 7) | ((u64)fst << 8) | ((u64)ctxt << 9) | ((u64)fix << 10));
}
