#include "common.h"

u64 GsPackFloat(f32 arg0) {
    union {
        f32 f;
        u32 i;
    } u;
    u.f = arg0;
    return u.i;
}
