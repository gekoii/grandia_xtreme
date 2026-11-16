/* Port-only C for VuQuatToMatrix (0x0043E6A8), which the decompilation
   still has in assembly (asm/main/main/03E6A8.s). Nothing calls it. */
#include "common.h"
#include "vu0_port.h"

void VuMtxRotateQuat(f32 *m, f32 *q);

void VuQuatToMatrix(f32 *m, f32 *q);

/* m = the identity, then rotated by quaternion q (VuMtxRotateQuat, the
   asm's tail call). The asm leaves $vf5-$vf7 as the identity's rows; the
   port doesn't write grx_vf. */
void VuQuatToMatrix(f32 *m, f32 *q) {
    VuUnitMtx(m);
    VuMtxRotateQuat(m, q);
}
