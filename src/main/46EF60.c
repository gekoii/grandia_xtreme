/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern s32 DeleteSema(s32);
extern void SignalSema(s32);
extern void DIntr(void);
extern void EIntr(void);

void _sceCdSemaExit(void) {
    if (_sceCdCbThreadId != 0) {
        _sceCdCbCmd = -1;
        do { SignalSema(_sceCdCbSema); } while (0);
    }
    DeleteSema(_sceCdNCmdSema);
    DeleteSema(_sceCdSCmdSema);
    DeleteSema(_sceCdCbSema);
    DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    EIntr();
}
