/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void Deci2Call(s32, s32 *);

void sceDeci2Poll(s32 arg0) {
    s32 buf[4];
    buf[0] = arg0;
    Deci2Call(4, buf);
}
