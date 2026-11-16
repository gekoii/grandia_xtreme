#include "common.h"
#include "protos/main.h"


void *MemAlloc16A(s32 arg0) {
    return MemAlloc(arg0, 0x10);
}
