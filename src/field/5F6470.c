#include "common.h"
#include "m2c_macros.h"

s32 ItemStubRet10IfZero(s32 arg0) {
    return ((arg0 << 0x10) != 0) ? 0 : 0xA;
}
