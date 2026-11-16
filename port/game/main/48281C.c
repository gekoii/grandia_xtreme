/* Port-only C for func_0048281C (0x0048281C), which the decompilation still has in assembly. */
/* An entry into memset (config/linker_script_extra.ld); memset is the host's */
#include "common.h"

void *memset(void *, s32, u32);

void *func_0048281C(void *dst, s32 c, u32 n) {
    return memset(dst, c, n);
}
