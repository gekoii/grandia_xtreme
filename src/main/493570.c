/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s64 sceSifLoadStartModuleBuffer(void) {
    return ((s32 (*)(void))_sceSifLoadModuleBuffer)();
}
