/* Port-only C for VuCopy128 (0x0045E4E8), which the decompilation still has in assembly. */
#include "common.h"

void VuCopy128(void *dst, void *src);

/* Copies 8 quadwords (0x80 bytes); all are loaded before any is stored */
void VuCopy128(void *dst, void *src) {
    GrxQuad q[8];

    __builtin_memcpy(q, src, sizeof(q));
    __builtin_memcpy(dst, q, sizeof(q));
}
