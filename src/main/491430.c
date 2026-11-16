/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 sceRmdir(s32 arg0) {
    return _sceFsPathCall(arg0, 8);
}
