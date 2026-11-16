#include "common.h"
#include "m2c_macros.h"
#include "ptcl.h"

void PtclDbgSetExtSizes(s32 arg0, s32 arg1, s32 arg2) {
    ptclDbgExtFileSize = arg0;
    ptclDbgExtSortSize = arg1;
    ptclDbgExtInstSize = arg2;
}
