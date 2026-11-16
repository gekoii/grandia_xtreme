/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void sceDmaPause(volatile u32 *arg0) {
    s32 v;

    if (*(volatile u32 *)0x1000F520 == 0) {
        v = 0x1000;
        *(volatile u32 *)0x1000F590 = v;
    }
    v = 0xFFFFFEFF & *arg0;
    *arg0 = v;
    v = 0;
    *(volatile u32 *)0x1000F590 = v;
}
