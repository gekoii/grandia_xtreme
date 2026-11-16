/* Port-only C for VuMtxBlend (0x004328B0), which the decompilation still
   has in assembly (asm/main/main/0328B0.s). Its caller is
   GphModelCalcPartsMotionBlend (port/game/main/4349F8.c), which passes
   1 - t and t. */
/*
 * The asm leaves $vf2/$vf3 = the weights and $vf4-$vf15 = the inputs and
 * results; no caller reads them, so the port doesn't write grx_vf.
 */
#include "common.h"

void VuMtxBlend(f32 *dst, f32 *a, f32 *b, f32 wa, f32 wb);

/* dst = a * wa + b * wb, element-wise over the 4x4 matrix */
void VuMtxBlend(f32 *dst, f32 *a, f32 *b, f32 wa, f32 wb) {
    f32 r[16];
    s32 i;

    for (i = 0; i < 16; i++) {
        r[i] = a[i] * wa + b[i] * wb;
    }
    __builtin_memcpy(dst, r, sizeof(r));
}
