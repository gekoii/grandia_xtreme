/* Port-only C for MemSet (0x0045C1B0), which the decompilation still has in assembly. */
#include "common.h"

void MemSet(void *dst, s32 val, s32 size);

/* Fills size bytes with the low byte of val. The asm stores bytes up to a
   16-byte boundary, then 32-byte blocks of quadwords (the byte spread
   across a register with pextlb/ppach/pcpyh), then the tail bytes; the
   memory written is the same as this. size is unsigned there. */
void MemSet(void *dst, s32 val, s32 size) {
    u8 *p = dst;
    u32 n = size;

    while (n != 0) {
        *p++ = val;
        n--;
    }
}
