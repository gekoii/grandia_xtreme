#include "common.h"
#include "field/n26.h"

void EvtPictureReqsReset(void) {
    s32 i;
    EvtPictureReq *p;
    s32 m1;
    s32 ff;
    s32 ff2;

    m1 = -1;
    ff = 0xFF;
    ff2 = 0xFF;
    p = evtPictureReqs;
    for (i = 2; i >= 0; i--) {
        p->id = m1;
        p->state = ff;
        p->pos = 0;
        p->unk6 = ff2;
        p->unk8 = ff2;
        p->unkA = 0;
        p++;
    }
}
