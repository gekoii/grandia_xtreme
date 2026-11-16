#include "common.h"
#include "field/n24.h"

s32 MiniGetTenantCount(void) {
    s32 r;
    if (miniCtl->dataFile == 0) {
        return 0;
    }
    r = 0x19;
    if (miniCtl->kind == 2) {
        r = 0;
    }
    return r;
}
