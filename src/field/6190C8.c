#include "common.h"
#include "field/n22.h"

s32 FldMemGetSubBuf(s32 arg0) {
    return (fldMemSubTop + 0x7F) & 0xFFFFFF80;
}
