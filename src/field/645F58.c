#include "common.h"
#include "field/n24.h"

void MiniStart(s32 arg0, u16 arg1, u16 arg2) {
    if (miniCtl->dataFile != 0 && miniCtl->state == 0) {
        miniCtl->kind = arg0;
        miniCtl->state = 1;
        miniCtl->startEvt = arg1;
        miniCtl->endEvt = arg2;
    }
}
