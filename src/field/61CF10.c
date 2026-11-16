#include "common.h"
#include "field/n22.h"

void MemSet(void *, s32, s32);

void FldEncGroupClear(void) {
    MemSet(&fldEncGroups, 0, sizeof(FldEncGroups));
}
