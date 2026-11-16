/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"



void memcpy(s32, ScePadData *, s32);

s32 scePadRead(s32 arg0, s32 arg1, s32 arg2) {
    ScePadData *r;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    r = _scePadGetDmaStr(arg0, arg1);
    memcpy(arg2, r, r->length);
    return r->length;
}
