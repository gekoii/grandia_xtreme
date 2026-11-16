/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"

s32 CreateSema(SemaParam *);

void _sceFsSemaInit(void) {
    SemaParam p;
    if (_sceFsFdSema == -1) {
        p.option = 0;
        p.initCount = 1;
        p.maxCount = 1;
        _sceFsFdSema = CreateSema(&p);
        _sceFsAsyncSema = CreateSema(&p);
    }
}
