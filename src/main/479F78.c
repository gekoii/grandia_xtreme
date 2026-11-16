/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"




u8 scePadGetState(s32 arg0, s32 arg1) {
    ScePadData *p;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0x63;
    }
    p = _scePadGetDmaStr(arg0, arg1);
    if (p->state == 6) {
        if (p->reqState == 2) {
            return 5;
        }
    }
    return p->state;
}
