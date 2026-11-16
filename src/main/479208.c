/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void DIntr(void);
void EIntr(void);

void _sceIpuSetFromChcr(s32 arg0) {
    DIntr();
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B000 = arg0;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & ~0x10000;
    EIntr();
}
