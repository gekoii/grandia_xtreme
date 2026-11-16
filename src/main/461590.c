#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndGetStatus(s32 arg0, SndStatus *info) {
    SndHandle *p;
    s32 ret;
    s32 m;

    if (info == NULL) {
        return -9;
    }
    {
        m = -1;
        info->stat = 0;
        info->cate = 0;
        info->disVol = 0;
        info->nowVol = 0;
        info->kTime = m;
        info->atr = m;
        p = SndFindHandle(arg0);
        if (p == NULL) {
            return -3;
        }
        info->stat = p->state;
        if (p->state == 3) {
            info->kTime = p->timer;
        }
        info->cate = p->unk6;
        info->atr = p->type;
        info->disVol = p->targetVol;
        info->nowVol = p->curVol;
        switch (p->type) {
        case 5:
        case 10:
            info->error = p->voice;
            break;
        case 4:
            info->error = sndIopStatus->strmInfo[p->chan].state;
            break;
        default:
            return 0;
        }
        ret = 0;
    }
    return ret;
}
