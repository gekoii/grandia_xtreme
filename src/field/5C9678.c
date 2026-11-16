#include "common.h"
#include "m2c_macros.h"

extern s32 fldInputLock;

void FldInputLockSet(s32 arg0) {
    fldInputLock = arg0 & 0xFF;
}

s32 FldInputLockGet(void) {
    return fldInputLock;
}
