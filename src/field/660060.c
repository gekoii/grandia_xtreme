#include "common.h"
#include "field/event.h"
#include "protos/field.h"

void EvtScnrWorkReset(void) {
    evtScnrWork->flags = 0;
    evtScnrWork->pc = 0;
    evtScnrWork->index = 0;
    evtScnrWork->body = 0;
    evtScnrWork->unk44A = 0;
    evtScnrWork->textColor = 0;
    evtScnrWork->waitTimer = 0;
    evtScnrWork->idBase = 0;
    evtScnrWork->lastWinCode = 0;
    EvtScnrRegSlotsReset();
    EvtScnrTempFlagsReset();
    EvtScnrAreaReset();
    EvtSndSlotsResetAll();
    EvtMsgFrameLayoutInit();
}
