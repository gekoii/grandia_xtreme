/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "protos/main.h"

void WaitSema(s32);

s32 _sceFsLock(s32 arg0) {
    _sceFsLockInit();
    WaitSema(_sceFsCallSema);
    return 0;
}
