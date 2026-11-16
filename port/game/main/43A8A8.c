/* Port-only C for GphShadowCalcMtx (0x0043A8A8), which the decompilation still has in assembly. */
#include "common.h"
#include "vu0_port.h"

void GphShadowCalcMtx(f32 *m, f32 *light);

/* Projection onto y = 0 along the light direction: the identity with
 * row 1 = (-l.x / l.y, 0, -l.z / l.y, 0) */
void GphShadowCalcMtx(f32 *m, f32 *light) {
    VuUnitMtx(m);
    m[5] = 0.0f;
    m[4] = -light[0] / light[1];
    m[6] = -light[2] / light[1];
}
