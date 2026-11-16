/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"

s32 iSignalSema(s32);

void _sceCdGenericCallback(s32 *arg0) {
    (*(volatile s32 *)&_sceCdCbCmd) = *arg0;
    sceCdCbfunc_num = (*(volatile s32 *)&_sceCdCbCmd);
    if ((*(volatile s32 *)&_sceCdCbCmd) == 0xB) {
        (*(volatile s32 *)&_sceCdCbCmd) = 0;
        _sceCdNCmdBusy = 0;
        return;
    }
    iSignalSema(_sceCdNCmdSema);
    if (_sceCdCbThreadId != 0 && sceCdCbfunc != 0) {
        iSignalSema(_sceCdCbSema);
    } else {
        _sceCdNCmdBusy = 0;
    }
    (*(volatile s32 *)&_sceCdCbCmd) = 0;
}
