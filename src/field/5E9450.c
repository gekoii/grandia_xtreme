#include "common.h"
#include "field/n17.h"

void MemSet(void *, s32, s32);

void FldSkyInit(void) {
    MemSet(fldSkyTable, 0, sizeof(fldSkyTable));
}
