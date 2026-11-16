/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

extern void DIntr(void);
extern void EIntr(void);

void _sceIpuSetToChcr(s32 arg0) {
    DIntr();
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B400 = arg0;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & ~0x10000;
    EIntr();
}
