/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

M2C_UNK Exit(s32);                         /* extern */

s32 getpid(void) {
    return 1;
}

s32 kill(s32 arg0, s32 arg1) {
    if (arg0 == 1) {
        Exit(arg1);
    }
    return 0;
}

s32 _sceGlueStubEio2(void) {
    *__errno() = 5;
    return -1;
}

s32 _sceGlueStubEio3(void) {
    *__errno() = 5;
    return -1;
}
