#include "common.h"
#include "field/event.h"

void EvtScnrStateReset(void) {
    evtScnrWork->unk44A = 0;
    evtScnrWork->waitTimer = 0;
    evtScnrWork->streamWait = 0;
    evtScnrWork->textColor = 0;
    evtScnrWork->lastWinCode = 0xFF;
    evtScnrWork->unk46C = 0;
    evtScnrWork->pc = 0;
}
