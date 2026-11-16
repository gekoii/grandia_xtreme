/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"



void sceSifRemoveCmdHandler(s32 arg0) {
    if (arg0 < 0) {
        _sceSifCmdDataSysHandlers[arg0].handler = 0;
    } else {
        _sceSifCmdDataUsrHandlers[arg0].handler = 0;
    }
}
