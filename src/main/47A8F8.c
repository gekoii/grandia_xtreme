/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"


s32 scePadExitPressMode(s32 arg0, s32 arg1) {
    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(arg0, arg1, 0);
}
