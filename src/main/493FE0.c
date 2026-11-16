/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void Deci2Call(s32, s32 *);

void kputs(s32 arg0, s32 arg1) {
    Deci2Call(0x10, &arg0);
}
