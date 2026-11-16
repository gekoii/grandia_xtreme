/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void SetVSyncFlag(volatile s32 *, volatile s64 *);
s32 DIntr(void);
void EIntr(void);

s64 _sceWaitVSyncCsr(void) {
    volatile s32 flag;
    volatile s64 val;
    s32 r;

    flag = 0;
    SetVSyncFlag(&flag, &val);
    r = DIntr();
    *(volatile s32 *)0x1000F000 = 4;
    __asm__ volatile("sync");
    if (r != 0) {
        EIntr();
    }
    while (!(*(volatile s32 *)0x1000F000 & 4) && flag == 0) {
    }
    r = DIntr();
    *(volatile s32 *)0x1000F000 = 4;
    __asm__ volatile("sync");
    if (r != 0) {
        EIntr();
    }
    return val;
}
