/* Port-only C for VuMtxSetRotX (0x0045D8C8), which the decompilation still has in assembly. */
/*
 * Also VuMtxSetRotY (0x0045D930) and VuMtxSetRotZ (0x0045D998): the
 * identity with a rotation by a 16-bit angle (0x10000 = 2pi), sin and cos
 * from VuSinCosAngle.
 *
 * VU0 registers: the identity is built in $vf5-$vf7 on the PS2; no caller
 * reads them afterwards, so the port doesn't write grx_vf.
 */
#include "common.h"
#include "vu0_port.h"

void VuSinCosAngle(f32 *s, f32 *c, s32 angle);

void VuMtxSetRotX(f32 *m, s32 angle);
void VuMtxSetRotY(f32 *m, s32 angle);
void VuMtxSetRotZ(f32 *m, s32 angle);

void VuMtxSetRotX(f32 *m, s32 angle) {
    f32 sc[2];

    VuSinCosAngle(&sc[0], &sc[1], angle);
    VuUnitMtx(m);
    m[6] = sc[0];
    m[10] = sc[1];
    m[5] = sc[1];
    m[9] = -sc[0];
}

void VuMtxSetRotY(f32 *m, s32 angle) {
    f32 sc[2];

    VuSinCosAngle(&sc[0], &sc[1], angle);
    VuUnitMtx(m);
    m[2] = sc[0];
    m[10] = sc[1];
    m[0] = sc[1];
    m[8] = -sc[0];
}

void VuMtxSetRotZ(f32 *m, s32 angle) {
    f32 sc[2];

    VuSinCosAngle(&sc[0], &sc[1], angle);
    VuUnitMtx(m);
    m[1] = sc[0];
    m[5] = sc[1];
    m[0] = sc[1];
    m[4] = -sc[0];
}
