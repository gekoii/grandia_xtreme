#include "common.h"
#include "protos/main.h"


void *MemAlloc16B(s32 arg0) {
    return MemAlloc(arg0, 0x10);
}
