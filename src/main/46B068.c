#include "common.h"
#include "protos/main.h"


u32 *MovGsPrmode(u32 *arg0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8) {
    return MovGsAd(arg0, 0x1B,
                  (u64)a1 << 3 | (u64)a2 << 4 | (u64)a3 << 5 | (u64)a4 << 6 |
                  (u64)a5 << 7 | (u64)a6 << 8 | (u64)a7 << 9 | (u64)a8 << 10);
}
