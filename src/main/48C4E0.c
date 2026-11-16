/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 DIntr(void);
void EIntr(void);

void _sceWaitVSync(void) {
    s32 r;

    r = DIntr();
    *(volatile s32 *)0x1000F000 = 4;
    __asm__ volatile("sync");
    if (r != 0) {
        EIntr();
    }
    while (!(*(volatile s32 *)0x1000F000 & 4)) {
    }
    r = DIntr();
    *(volatile s32 *)0x1000F000 = 4;
    __asm__ volatile("sync");
    if (r != 0) {
        EIntr();
    }
}
