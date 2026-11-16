#include "common.h"
#include "snd.h"

s32 SignalSema(s32);
s32 WaitSema(s32);

void viBufEndPut(void *arg0, s32 arg1) {
    ViBuf *p = arg0;

    WaitSema(p->sema);
    p->readBytes += arg1;
    p->totalBytes += arg1;
    SignalSema(p->sema);
}
