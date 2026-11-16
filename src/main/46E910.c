/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern u8 _sceDmaSyncTimeoutStr[];

s32 sceDmaSync(volatile u32 *arg0, s32 arg1, s32 arg2) {
    s32 v;
    s32 w;

    if (arg1 == 1) {
        return (*arg0 >> 8) & 1;
    }
    if (arg2 == 0) {
        arg2 = 0x1000000;
    }
    while (*arg0 & 0x100) {
        if (--arg2 < 0) {
            printf(_sceDmaSyncTimeoutStr);
            v = *arg0;
            if (((u32)v >> 8) & 1) {
                do {
                    w = v & ~0x100;
                    v = w;
                } while (((u32)v >> 8) & 1);
                *arg0 = w;
            }
        }
    }
    return 0;
}
