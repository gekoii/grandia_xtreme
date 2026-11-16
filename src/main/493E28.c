/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
extern s32 _sceDeci2Work;
s32 Deci2Call(s32, void *);
s32 sceDeci2Open(u16 arg0, s32 arg1, s32 arg2) {
    s32 buf[4];
    buf[0] = arg0;
    buf[1] = arg1;
    buf[2] = arg2;
    buf[3] = (u32)&_sceDeci2Work | 0x20000000;
    return Deci2Call(1, buf);
}
