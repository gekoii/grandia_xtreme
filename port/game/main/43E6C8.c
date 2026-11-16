/* Port-only C for VuQuatToEuler (0x0043E6C8), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "protos/main.h"
#include "vu0_port.h"

void VuMtxRotateQuat(f32 *m, f32 *q);

void VuQuatToEuler(s32 *out, f32 *q);

/* The rotation of quaternion q as a matrix, then its euler angles */
void VuQuatToEuler(s32 *out, f32 *q) {
    FMATRIX m;

    VuUnitMtx(&m[0][0]);
    VuMtxRotateQuat(&m[0][0], q);
    VuMatrixToEuler(out, &m[0][0]);
}
