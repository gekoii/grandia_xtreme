/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "protos/main.h"


extern s32 SignalSema(s32);
extern s32 WaitSema(s32);

SceFsFd *_sceFsAllocFd(void) {
    SceFsFd *p;
    SceFsFd *ret;

    _sceFsSemaInit();
    WaitSema(_sceFsFdSema);
    for (p = _sceFsFdTable; p < &_sceFsFdTable[32]; p++) {
        if (p->flags == 0) {
            p->flags = 0x10000000;
            SignalSema(_sceFsFdSema);
            return p;
        }
    }
    SignalSema(_sceFsFdSema);
    return NULL;
}
