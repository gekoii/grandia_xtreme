#include "common.h"
#include "main/n09.h"

s32 DisableIntc(s32);
s32 RemoveIntcHandler(s32, s32);

void GsVsyncExit(void) {
    DisableIntc(2);
    RemoveIntcHandler(2, gsVblankHandlerId);
    *(volatile s32 *)0x10000800 = 0;
    gsFlipWaitCount = 0;
    gsVblanksSinceFlip = 0;
}
