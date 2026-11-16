/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _sceSioPutChar(s32 arg0) {
    if (arg0 == 0xA) {
        _sceSioPutRawChar(0xD);
        _sceSioPutRawChar(0xA);
    } else {
        _sceSioPutRawChar(arg0);
    }
}
