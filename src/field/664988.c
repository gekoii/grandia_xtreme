#include "common.h"
#include "field/event.h"

s32 EvtScnrIsDebug(void) {
    return evtScnrWork->flags & EVT_SCNR_DEBUG;
}

s32 EvtScnrIsLoaded(void) {
    return evtScnrCur->id != 0xFFFF;
}

void EvtScnrSetTextColor(s16 arg0) {
    evtScnrWork->textColor = arg0;
}
