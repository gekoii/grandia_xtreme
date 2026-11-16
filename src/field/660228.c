#include "common.h"
#include "protos/field.h"


void EvtScnrAreaReset(void) {
    EvtScnrStateReset();
    EvtScnrScriptCtlReset();
    EvtMsgCtlReset();
    EvtMsgWindowsReset();
    EvtSndSlotsReset();
    EvtMsgCursorReset();
    EvtScnrBlock790Reset();
    EvtMsgInputReset();
    EvtPictureReqsReset();
    EvtTenantPointReqsReset();
    EvtScnrBlock7D8Reset();
    EvtScnrOccWaitClear();
    EvtScnrOccWakeClear();
}
