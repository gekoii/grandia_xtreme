/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 Deci2Call(s32, void *);

s32 sceDeci2ExSend(s32 arg0, s32 arg1, u16 arg2) {
    s32 buf[3];
    buf[0] = arg0;
    buf[1] = arg1;
    buf[2] = arg2;
    return Deci2Call(-6, buf);
}
