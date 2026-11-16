#include "common.h"
#include "field/n27.h"

void EvtSndSlotsResetAll(void) {
    s32 i;
    EvtScnrSndStatus *e;
    s32 m1;

    m1 = -1;
    e = evtScnrSndStatus;
    for (i = 10; i >= 0; i--) {
        e->id = m1;
        e->unk4 = m1;
        e->state = 0;
        e->time = 0;
        e++;
    }
}
