#include "common.h"
#include "field/event.h"
#include "field/n20.h"

s32 EvtScnrHighIdLatch(void) {
    u16 v;

    if (evtScnrHighIdLatchFlag == 1) {
        return 1;
    }
    v = evtScnrCur->id;
    if (v == 0xFFFF) {
        return 0;
    }
    if (v <= 0xEFFF) {
        return 0;
    }
    evtScnrHighIdLatchFlag = 1;
    return 1;
}
