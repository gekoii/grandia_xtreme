/* Port-only C for GsVblankHandler (0x0045EB50), which the decompilation still has in assembly. */
#include "common.h"
#include "main/n09.h"
#include "protos/main.h"

s32 GsVblankHandler(s32);

/* The vblank interrupt handler (INTC 2, GsVsyncInit). The cause argument
   in $a0 isn't read. It ends with sync and ei, which the port has no
   equivalent for. */
s32 GsVblankHandler(s32 cause) {
    gsVblanksSinceFlip++;
    if (gsFlipWaitCount != 0) {
        gsFlipWaitCount--;
        if (gsFlipWaitCount == 0) {
            *(volatile s32 *)0x10000800 = 0;
            gsFlipWaitCount = 0;
            gsVblanksSinceFlip = 0;
        }
    } else {
        gsVblankIdleCount++;
    }
    gsVblankCount++;
    SndUpdate();
    grx_sync();
    return 0;
}
