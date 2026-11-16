/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

u32 sceIpuSync(s32 arg0, s32 arg1) {
    u32 r = 0;

    switch (arg0) {
    case 0:
        while (*(volatile s32 *)0x10002010 < 0) {
        }
        r = 0;
        break;
    case 1:
        r = *(volatile u32 *)0x10002010 >> 31;
        break;
    }
    return r;
}
