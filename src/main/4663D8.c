#include "common.h"

void setD4_CHCR(s32 arg0) {
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B400 = arg0;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & 0xFFFEFFFF;
    if (!(arg0 & 0x100)) {
        __asm__ volatile("sync");
    }
}
