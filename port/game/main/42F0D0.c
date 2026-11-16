/* Port-only C for VuSinCosRad (0x0042F0D0), which the decompilation still has in assembly. */
/*
 * Also VuSinCosDeg (0x0042F118): the angle converted to 16-bit units
 * (vftoi0, saturating) for VuSinCosAngle.
 */
#include "common.h"
#include "vu0_port.h"

void VuSinCosAngle(f32 *s, f32 *c, s32 angle);

void VuSinCosRad(f32 *s, f32 *c, f32 rad);
void VuSinCosDeg(f32 *s, f32 *c, f32 deg);

/* 65536 / 2pi */
void VuSinCosRad(f32 *s, f32 *c, f32 rad) {
    VuSinCosAngle(s, c, VuFtoi(rad * VuBits(0x4622F983), 0));
}

/* 65536 / 360 */
void VuSinCosDeg(f32 *s, f32 *c, f32 deg) {
    VuSinCosAngle(s, c, VuFtoi(deg * VuBits(0x43360B61), 0));
}
