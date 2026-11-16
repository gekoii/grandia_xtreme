#include "common.h"
#include "pad.h"
#include "protos/main.h"

s32 scePadPortOpen(s32, s32, void *);
extern s32 padDmaBuf0;
extern s32 padDmaBuf1;

s32 PadInit(void) {
    scePadInit(0);
    padVibEnabled = 1;
    padInputLocked = 0;
    if (scePadPortOpen(0, 0, &padDmaBuf0) == 0) {
        return 0;
    }
    if (scePadPortOpen(1, 0, &padDmaBuf1) == 0) {
        return 0;
    }
    return 1;
}
