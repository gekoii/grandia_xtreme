#include "common.h"

void MovGifSendChain(u32 arg0) {
    *(volatile u32 *)0x1000A030 = arg0 & 0x0FFFFFFF;
    *(volatile u32 *)0x1000A020 = 0;
    *(volatile u32 *)0x1000A000 = 0x105;
}
