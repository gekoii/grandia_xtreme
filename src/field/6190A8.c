#include "common.h"
#include "field/n22.h"

s32 FldMemGetTailBuf(s32 arg0) {
    return ((fldMemEnd - arg0) - 2) & 0xFFFFFF80;
}
