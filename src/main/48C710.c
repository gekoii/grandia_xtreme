/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


s32 _sceGlueStubEio1(void) {
    *__errno() = 5;
    return -1;
}

s32 close(s32 arg0) {
    return -1;
}

s32 _sceGlueStubFail(void) {
    return -1;
}

s32 lseek(s32 arg0, s32 arg1, s32 arg2) {
    return -1;
}
