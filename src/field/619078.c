#include "common.h"
#include "field/n22.h"

s32 FldMemTailOverlaps(s32 arg0) {
    return (u32)(((fldMemEnd - arg0) - 2) & 0xFFFFFF80) < fldMemCur;
}
