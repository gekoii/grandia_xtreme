/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

extern void DIntr(void);
extern void EIntr(void);

void _sceIpuInitSetToChcr(u32 arg0) {
    DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B400 = arg0;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & ~0x10000;
    EIntr();
}
