/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"





s32 scePadSetReqState(s32 arg0, s32 arg1, s32 arg2) {
    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    _scePadGetDmaStr(arg0, arg1)->reqState = arg2;
    return 1;
}
