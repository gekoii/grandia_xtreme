/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"


s32 sceSifSetSysCmdBuffer(s32 arg0, s32 arg1) {
    s32 old;

    old = (s32)_sceSifCmdData.sys_cmd_handlers;
    _sceSifCmdData.sys_cmd_handlers = (sceSifCmdHandlerData *)arg0;
    _sceSifCmdData.nr_sys_handlers = arg1;
    return old;
}
