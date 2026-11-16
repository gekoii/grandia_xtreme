#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"

void FldMenuMcCheckExit(void) {

}

s32 FldMenuMcCheckUpdate(void) {
    if (McSpaceCheckUpdate() == 1) {
        FldMenuClose();
    }
    return 0xB;
}
