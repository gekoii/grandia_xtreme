/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "protos/main.h"

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);

SceFsFd *_sceFsGetFd(u32 arg0) {
    SceFsFd *p;
    _sceFsSemaInit();
    WaitSema(_sceFsFdSema);
    if (arg0 >= 0x20) {
        SignalSema(_sceFsFdSema);
        return NULL;
    }
    p = &_sceFsFdTable[arg0];
    SignalSema(_sceFsFdSema);
    return p;
}
