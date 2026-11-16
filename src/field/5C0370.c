#include "common.h"

void *FldMemAlloc(s32);

void *MapWallAllocCylinder(void) {
    return FldMemAlloc(0x30);
}
