#include "common.h"
#include "field/n26.h"

void EvtScnrRegSlotsReset(void) {
    s32 i;
    EvtScnrRegSlot *p;
    s32 ff;

    ff = 0xFF;
    p = evtScnrRegSlots;
    for (i = 7; i >= 0; i--) {
        p->id = ff;
        p->unk4 = 0;
        p->unkC = 0;
        p++;
    }
}
