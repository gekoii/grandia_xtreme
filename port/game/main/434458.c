/* Port-only C for GphSkinPackNormals (0x00434458), which the decompilation still has in assembly. */
#include "common.h"
#include "vu0_port.h"

void GphSkinPackNormals(s16 *dst, f32 *src, s32 n);

/*
 * Packs float vectors into 4.12 fixed-point s16 x 4 (vftoi12, then the low
 * half of each word), four vectors per step: n is rounded up to a multiple
 * of 4.
 */
void GphSkinPackNormals(s16 *dst, f32 *src, s32 n) {
    s32 groups = (n + 3) >> 2;
    s32 k;
    s32 i;

    for (; groups > 0; groups--) {
        for (k = 0; k < 4; k++) {
            s32 w[4];

            for (i = 0; i < 4; i++) {
                w[i] = VuFtoi(src[k * 4 + i], 12);
            }
            VuPackH4(&dst[k * 4], w);
        }
        src += 16;
        dst += 16;
    }
}
