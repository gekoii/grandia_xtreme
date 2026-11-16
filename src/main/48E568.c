/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


u32 sceSifSendCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    return _sceSifSendCmd(arg0, 0, arg1, arg2, arg3, arg4, arg5);
}
