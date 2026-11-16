#include "common.h"
#include "field/n22.h"

s32 FldMemReset(void) {
    return fldMemCur = fldMemTop;
}
