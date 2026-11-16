#include "common.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void FldMenuWinTextAddIndexed(u8 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    if (arg7 >= 2) {
        FldMenuWinTextAdd(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7 + 0x18, arg7 + 0xF, arg8);
        return;
    }
    FldMenuWinTextAdd(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7 + 0xC, arg7 + 0xF, arg8);
}
