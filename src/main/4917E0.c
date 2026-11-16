/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s64 sceDelDrv(s32 arg0) {
    return _sceFsPathCall(arg0, 0x10);
}
