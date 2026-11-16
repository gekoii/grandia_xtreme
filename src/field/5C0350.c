#include "common.h"

void *FldMemAlloc(s32);

void *MapWallAllocChain(s32 arg0) {
    return FldMemAlloc(arg0 * 16 + 0x20);
}
