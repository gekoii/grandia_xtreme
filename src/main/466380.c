#include "common.h"

void setD3_CHCR(u32 arg0) {
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B000 = arg0;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & ~0x10000;
    if (!(arg0 & 0x100)) {
        asm volatile("sync");
    }
}
