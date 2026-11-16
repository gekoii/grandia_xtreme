#include "common.h"
#include "protos/main.h"


s32 MemAlloc32(s32 arg0) {
    return (s32)MemAlloc(arg0, 0x20);
}
