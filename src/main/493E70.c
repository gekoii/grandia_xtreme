/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void Deci2Call(s32, void *);

void sceDeci2Close(s32 arg0) {
    Deci2Call(2, &arg0);
}
