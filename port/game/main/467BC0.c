/* Port-only C for SndFloatToInt (0x00467BC0), which the decompilation still has in assembly. */
#include "common.h"

/* cvt.w.s: the EE's FPU truncates, and saturates out-of-range values */
s32 SndFloatToInt(f32 x) {
    if (x >= 2147483648.0f) {
        return 0x7FFFFFFF;
    }
    if (x <= -2147483648.0f) {
        return (s32)0x80000000;
    }
    return (s32)x;
}
