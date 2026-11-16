#include "common.h"
#include "protos/main.h"

void GsMfifoSend(void *, s32);

void GsVif1MarkSend(s32 arg0) {
    GsFromSprDmaWait();
    *(u32 *)0x7000081C = (arg0 & 0xFFFF) | 0x07000000;
    *(u64 *)0x70000800 = 0x10000001;
    *(u64 *)0x70000808 = 0xEEEE0000ULL;
    *(u32 *)0x70000810 = 0;
    *(u32 *)0x70000814 = 0;
    *(u32 *)0x70000818 = 0;
    GsMfifoSend((void *)0x70000800, 2);
}
