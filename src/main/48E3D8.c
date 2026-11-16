/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
void sceSifAddCmdHandler(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 0) {
        _sceSifCmdDataSysHandlers[arg0].handler = (sceSifCmdHandler)arg1;
        _sceSifCmdDataSysHandlers[arg0].harg = (void *)arg2;
    } else {
        _sceSifCmdDataUsrHandlers[arg0].handler = (sceSifCmdHandler)arg1;
        _sceSifCmdDataUsrHandlers[arg0].harg = (void *)arg2;
    }
}
