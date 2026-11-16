/* Port-only C for VuDirFromAngles (0x00430738), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "protos/main.h"
#include "vu0_port.h"

extern const FVECTOR vuForwardVec; /* (0, 0, 1, 1) */

void VuDirFromAngles(f32 *out, f32 *angles);

/*
 * out.xyz = vuForwardVec rotated by angles[0] about X, then angles[1]
 * about Y (degrees, through the 16-bit angle units of VuMtxRotX/Y).
 * out[3] is not written.
 */
void VuDirFromAngles(f32 *out, f32 *angles) {
    FMATRIX unit;
    FMATRIX rx;
    FMATRIX rxy;
    f32 a[4];
    s32 ia[4];
    f32 v[4];
    s32 i;

    VuUnitMtx(&unit[0][0]);
    a[0] = angles[0] * VuBits(0x3C8EFA35); /* pi / 180 */
    a[1] = angles[1] * VuBits(0x3C8EFA35);
    a[2] = 0.0f;
    a[3] = 0.0f;
    for (i = 0; i < 4; i++) {
        ia[i] = VuFtoi(a[i] * VuBits(0x4622F983), 0); /* 65536 / 2pi */
    }
    VuMtxRotX((s32)rx, ia[0], (s32)unit);
    VuMtxRotY((s32)rxy, ia[1], (s32)rx);
    VuApplyMtx(v, vuForwardVec, &rxy[0][0]);
    out[0] = v[0];
    out[2] = v[2];
    out[1] = v[1];
}
