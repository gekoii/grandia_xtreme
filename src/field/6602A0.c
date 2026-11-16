#include "common.h"
#include "field/event.h"


void EvtScnrScriptCtlReset(void) {
    evtScnrCur->id = 0xFFFF;
    evtScnrCur->flags = 1;
    evtScnrCur->winKind = 0xFF;
    evtScnrCur->unk5 = 0xFF;
    evtScnrCur->unk6 = 0xF;
    evtScnrCur->condResult = 1;
    evtScnrCur->unk9 = 0;
    evtScnrCur->pc = 0;
    evtScnrCur->noticeTimer = 0;
}
