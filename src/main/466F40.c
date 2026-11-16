#include "common.h"
#include "snd.h"

s32 SignalSema(s32);
s32 WaitSema(s32);

s32 viBufIsActive(void *arg0) {
    ViBuf *p = arg0;
    s32 ret;

    WaitSema(p->sema);
    ret = p->isActive;
    SignalSema(p->sema);
    return ret;
}

s32 viBufCount(void *arg0) {
    ViBuf *p = arg0;
    s32 ret;

    WaitSema(p->sema);
    ret = (p->dmaN << 11) + p->readBytes;
    SignalSema(p->sema);
    return ret;
}
