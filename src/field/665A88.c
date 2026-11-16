#include "common.h"
#include "field/n26.h"

/* local prototype: with the header's (u16, u8, u8) the arguments are masked and registers differ (21 words) */
void EvtScnrPictureSet(s32, s32, s32);

void EvtScnrPictureRequest(u8 arg0, u8 arg1) {
    u16 idx = arg1 != 0;
    EvtPictureReq *e;

    e = evtPictureReqs;
    e += idx;
    if (arg0 == 0) {
        if (e->pos == arg1) {
            e->state = 2;
            e->id = 0;
            e->unk6 = 0xFF;
            e->unk8 = 0xFF;
            e->unkA = 0;
        }
    } else if (e->state == 0xFF) {
        EvtScnrPictureSet(idx, arg0, arg1);
    } else if (e->pos == arg1) {
        EvtScnrPictureSet(idx, arg0, e->pos);
    }
}
