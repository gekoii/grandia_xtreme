/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern s32 WaitSema(s32);
extern s32 ExitDeleteThread(void);
extern u8 _sceCdCbfuncFmt[];

void _sceCdCallbackThread(void) {
    while (1) {
        WaitSema(_sceCdCbSema);
        if (_sceCdCbCmd == -1) {
            _sceCdNCmdBusy = 0;
            _sceCdCbCmd = 0;
            _sceCdCbThreadId = 0;
            _sceCdCbThreadFlag = 0;
            ExitDeleteThread();
        }
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdCbfuncFmt, sceCdCbfunc, sceCdCbfunc_num);
        }
        if (sceCdCbfunc != NULL && sceCdCbfunc_num != 0) {
            sceCdCbfunc(sceCdCbfunc_num);
        }
        _sceCdNCmdBusy = 0;
    }
}
