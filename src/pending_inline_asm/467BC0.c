#include "common.h"

s32 func_00467BC0(f32 x) {
    s32 r;
    __asm__ volatile("trunc.w.s %1, %1\n\tmfc1 %0, %1" : "=r"(r) : "f"(x));
    return r;
}
