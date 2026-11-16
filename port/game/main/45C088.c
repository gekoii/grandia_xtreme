/* Port-only C for MemMove128 (0x0045C088), which the decompilation still has in assembly. */
#include "common.h"

s32 MemMove128(void *dst, void *src, u32 count);

/* Copies count quadwords (lq/sq), overlap-safe: forwards unless dst is
   above src (a signed difference), backwards from the end otherwise. The
   asm moves 8 quadwords per step, loading all 8 before storing, then the
   rest one at a time; the result is the same. Returns 0 ($v0 is the spent
   tail count); most callers declare it void. */
s32 MemMove128(void *dst, void *src, u32 count) {
    GrxQuad *d = dst;
    GrxQuad *s = src;
    GrxQuad q;
    u32 i;

    if ((s32)((u32)dst - (u32)src) > 0) {
        for (i = count; i != 0; i--) {
            __builtin_memcpy(&q, &s[i - 1], 16);
            __builtin_memcpy(&d[i - 1], &q, 16);
        }
    } else {
        for (i = 0; i < count; i++) {
            __builtin_memcpy(&q, &s[i], 16);
            __builtin_memcpy(&d[i], &q, 16);
        }
    }
    return 0;
}
