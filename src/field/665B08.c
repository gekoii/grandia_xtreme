#include "common.h"
#include "field/n26.h"

void EvtScnrPictureSet(u16 arg0, u8 arg1, u8 arg2) {
    EvtPictureReq *e;
    e = evtPictureReqs;
    e += arg0;

    if (e->id != arg1 || e->pos != arg2 || e->state != 1) {
        if (arg1 != 0 && arg2 == 0 && e->id != arg1) {
            if (e->unk8 == 0xFF) {
                e->unk8 = 0;
            } else {
                e->unk8++;
            }
        } else {
            e->unkA = 0;
        }
        e->state = 1;
        e->id = arg1;
        e->pos = arg2;
        if (e->unk6 == 0) {
            e->unk6 = 0;
        } else {
            e->unk6++;
        }
    }
}
