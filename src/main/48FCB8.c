/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"

void SignalSema(s32);

void _sceFsUnlock(void) {
    SignalSema(_sceFsCallSema);
}
