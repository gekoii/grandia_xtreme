/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void Deci2Call(s32, void *);

void sceDeci2ExUnLock(s32 arg0) {
    s32 buf[4];
    buf[0] = arg0;
    Deci2Call(-9, buf);
}
