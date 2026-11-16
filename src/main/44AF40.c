#include "common.h"
#include "ptcl.h"




PtclStarterSlot *PtclStarterSlotAlloc(PtclCtrl *arg0) {
    s32 n = ptclCurSystem->slotCount;
    PtclStarterSlot *s = ptclCurSystem->slots;
    while (n > 0 && s->ctrl != 0) {
        n--;
        s++;
    }
    if (n <= 0) {
        return NULL;
    }
    s->ctrl = arg0;
    s->gen = 0;
    s->time = 0;
    return s;
}
