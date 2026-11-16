#include "common.h"
#include "protos/main.h"


s32 MemAlloc64(s32 arg0) {
    return MemAlloc(arg0, 0x40);
}
