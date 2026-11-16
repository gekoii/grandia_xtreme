#include "common.h"
#include "field/event.h"

void EvtScnrOccWakeRequest(s16 arg0, s8 arg1) {
    evtScnrWork->occWakeKind = arg1;
    evtScnrWork->occWakeId = arg0;
}

s8 EvtScnrOccWakeGetKind(void) {
    return evtScnrWork->occWakeKind;
}

s16 EvtScnrOccWakeGetId(void) {
    return evtScnrWork->occWakeId;
}

void EvtScnrOccWakeClear(void) {
    evtScnrWork->occWakeKind = -1;
    evtScnrWork->occWakeId = -1;
}

void EvtScnrOccWaitSet(s8 arg0, s8 arg1, s16 arg2) {
    evtScnrWork->occWaitState = arg0;
    evtScnrWork->occWaitKind = arg1;
    evtScnrWork->occWaitId = arg2;
}

s8 EvtScnrOccWaitGetState(void) {
    return evtScnrWork->occWaitState;
}

s8 EvtScnrOccWaitGetKind(void) {
    return evtScnrWork->occWaitKind;
}

s16 EvtScnrOccWaitGetId(void) {
    return evtScnrWork->occWaitId;
}

void EvtScnrOccWaitClear(void) {
    evtScnrWork->occWaitState = -1;
    evtScnrWork->occWaitKind = -1;
    evtScnrWork->occWakeId = 0;
}
