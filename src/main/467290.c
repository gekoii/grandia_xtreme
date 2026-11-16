#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 SignalSema(s32);
s32 WaitSema(s32);

s32 viBufPutTs(void *arg0, void *arg1) {
    ViBuf *p = arg0;
    MovTimeStamp *ts = arg1;
    s32 ret;

    ret = 0;
    WaitSema(p->sema);
    if (p->countTs < p->nTs) {
        viBufModifyPts(p, ts);
        ret = 1;
        p->ts[p->wtTs].pts = ts->pts;
        p->ts[p->wtTs].dts = ts->dts;
        p->ts[p->wtTs].pos = ts->pos;
        p->ts[p->wtTs].len = ts->len;
        p->countTs++;
        p->wtTs = (p->wtTs + 1) % p->nTs;
    }
    SignalSema(p->sema);
    return ret;
}
