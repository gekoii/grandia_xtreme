/* Port-only C for VuFloatToInt (0x0045BE08), which the decompilation still has in assembly. */
#include "common.h"
#include "vu0_port.h"

s32 VuFloatToInt(f32 x);

/* vftoi0: truncates, saturating */
s32 VuFloatToInt(f32 x) {
    return VuFtoi(x, 0);
}
