/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"





s32 scePadGetFrameCount(s32 a, s32 b) {
    if (_scePadPortTable[a][b].open == 0) {
        return 0;
    }
    return _scePadGetDmaStr(a, b)->frame;
}
