#include "common.h"
#include "protos/viewer.h"

s32 VwrEntry(void) {
    SysProgRegister(VwrInit, VwrResume, VwrLoad, VwrUpdate,
                VwrBuildDrawLists, VwrDraw, VwrEndFrame, 0);
    return 0;
}
