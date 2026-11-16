/* Port-only C for MemMove32 (0x0045BE38), which the decompilation still has in assembly. */
#include "common.h"

s32 MemMove32(void *dst, void *src, u32 count);

/* Copies count words, overlap-safe: forwards unless dst is above src (a
   signed difference), backwards from the end otherwise. The asm moves
   8 words per step, loading all 8 before storing, then the rest one at a
   time; the result is the same. Returns 0 ($v0 is the spent tail count),
   though the callers declare it void. */
s32 MemMove32(void *dst, void *src, u32 count) {
    u32 *d = dst;
    u32 *s = src;
    u32 i;

    if ((s32)((u32)dst - (u32)src) > 0) {
        for (i = count; i != 0; i--) {
            d[i - 1] = s[i - 1];
        }
    } else {
        for (i = 0; i < count; i++) {
            d[i] = s[i];
        }
    }
    return 0;
}
