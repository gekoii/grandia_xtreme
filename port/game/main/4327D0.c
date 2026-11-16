/* Port-only C for VuMtxNormalizeAxes (0x004327D0), which the decompilation still has in assembly. */
#include "common.h"
#include "vu0_port.h"

void VuMtxNormalizeAxes(f32 *dst, f32 *src);

/* Rows 0-2 normalised (xyz; w kept), row 3 copied */
void VuMtxNormalizeAxes(f32 *dst, f32 *src) {
    f32 m[16];
    s32 i;

    VuCopyMtx(m, src);
    for (i = 0; i < 3; i++) {
        f32 *r = &m[i * 4];
        f32 q = VuRsqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

        r[0] *= q;
        r[1] *= q;
        r[2] *= q;
    }
    VuCopyMtx(dst, m);
}
