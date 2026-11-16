/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 sceDmaPutStallAddr(u32 arg0) {
    s32 old = *(volatile s32 *)0x1000E060;
    if (arg0 != 0xFFFFFFFF) {
        *(volatile s32 *)0x1000E060 = arg0;
    }
    return old;
}
