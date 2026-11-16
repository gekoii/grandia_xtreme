/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "sdk/eekernel.h"

s32 CreateSema(SemaParam *);

void _sceCdSemaInit(void) {
    SemaParam a;

    if (_sceCdNCmdSema == -1 || _sceCdSCmdSema == -1) {
        a.option = 0;
        a.initCount = 1;
        a.maxCount = 1;
        _sceCdNCmdSema = CreateSema(&a);
        _sceCdSCmdSema = CreateSema(&a);
        a.initCount = 0;
        _sceCdCbSema = CreateSema(&a);
        _sceCdNCmdBusy = 0;
    }
}
