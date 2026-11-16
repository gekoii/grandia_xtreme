/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void sceChdir(s32 arg0) {
    do {
        _sceFsPathCall(arg0, 0x12);
    } while (0);
}
