#include "common.h"
#include "field/event.h"

s32 EvtScnrIsRunning(void) {
    u16 id;

    id = evtScnrCur->id;
    if (id == 0xFFFF) {
        return 0;
    }
    return (id & 0xF000) != 0xE000;
}
