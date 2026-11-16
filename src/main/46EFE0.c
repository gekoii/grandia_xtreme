/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"
#include "m2c_macros.h"

M2C_UNK DIntr();                            /* extern */
M2C_UNK EIntr();                            /* extern */

s32 sceCdPOffCallback(s32 arg0, s32 arg1) {
    s32 temp_s0;

    if (_sceCdSifCmdBind < 0) {
        _sceCdPOffInit();
    }
    DIntr();
    temp_s0 = (s32)_sceCdSifCmdCbFunc;
    _sceCdSifCmdCbArg = (void *)arg1;
    _sceCdSifCmdCbFunc = (void (*)(void *))arg0;
    EIntr();
    return temp_s0;
}
