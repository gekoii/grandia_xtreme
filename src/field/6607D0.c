#include "common.h"
#include "field/n26.h"

void EvtScnrRegSlotAdd(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    EvtScnrRegSlot *p;
    s32 ff;

    i = 0;
    p = evtScnrRegSlots;
    ff = 0xFF;
    do {
        i++;
        if (p->id == ff) {
            p->id = arg0;
            p->unk4 = arg1;
            p->unkC = arg2;
            return;
        }
        p++;
    } while (i < 8);
}
