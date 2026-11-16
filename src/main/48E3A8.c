/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"



s32 sceSifSetCmdBuffer(s32 arg0, s32 arg1) {
    sceSifCmdCtrl *p = &_sceSifCmdData;
    s32 old = (s32)p->usr_cmd_handlers;
    p->usr_cmd_handlers = (sceSifCmdHandlerData *)arg0;
    p->nr_usr_handlers = arg1;
    return old;
}
