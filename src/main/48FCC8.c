/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "protos/main.h"


void DIntr(void);
void EIntr(void);

s32 _sceFsSetCallback(s32 arg0, s32 arg1) {
    SceFsCallback *p = &_sceFsIopCallback;
    s32 old;

    _sceFsLock(0x1B);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    DIntr();
    old = (s32)p->func;
    p->arg = (void *)arg1;
    p->func = (void (*)(void *))arg0;
    EIntr();
    _sceFsUnlock();
    return old;
}
