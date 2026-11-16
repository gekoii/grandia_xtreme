/* Port-only C for VuVecCopy (0x0045DA20), which the decompilation still has in assembly. */
#include "common.h"

void VuVecCopy(void *dst, void *src);

/* Copies x, y, z (an ld and an lw), leaving dst's w alone */
void VuVecCopy(void *dst, void *src) {
    u64 xy;
    u32 z;

    __builtin_memcpy(&xy, src, 8);
    z = ((u32 *)src)[2];
    __builtin_memcpy(dst, &xy, 8);
    ((u32 *)dst)[2] = z;
}
