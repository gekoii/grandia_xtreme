#include "common.h"
#include "protos/main.h"


s32 MemAlloc128(s32 arg0) {
    return (s32)MemAlloc(arg0, 0x80);
}
