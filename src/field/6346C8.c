#include "common.h"
#include "protos/field.h"

s32 FldMenuSaveUpdate(void) {
    s32 r;

    r = 0;
    if (McSaveGetState() == 0x800) {
        r = 1;
        FldMenuClose();
    }
    McSaveFlowUpdate();
    return r != 0 ? 0xFF : 9;
}
