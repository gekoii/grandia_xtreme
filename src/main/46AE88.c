#include "common.h"
#include "protos/main.h"


u32 *MovGsRgbaq(u32 *p, u32 r, u32 g, u32 b, u32 a, u32 q) {
    return MovGsAd(p, 1, (u64)r | ((u64)g << 8) | ((u64)b << 16) | ((u64)a << 24) | ((u64)q << 32));
}
