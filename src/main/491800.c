/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"


s32 SignalSema(s32);
s32 WaitSema(s32);

s32 sceDopen(s32 arg0) {
    s32 ret;
    SceFsFd *e;
    s32 sema;

    _sceFsLock(9);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    _sceFsUnlock();
    e = _sceFsAllocFd();
    if (e == NULL) {
        return -0x13;
    }
    ret = _sceFsPathCall(arg0, 9);
    if (ret < 0) {
        WaitSema(_sceFsFdSema);
        e->flags = 0;
        sema = _sceFsFdSema;
    } else {
        WaitSema(_sceFsFdSema);
        e->fd = ret;
        sema = _sceFsFdSema;
        ret = e - _sceFsFdTable;
    }
    SignalSema(sema);
    return ret;
}
