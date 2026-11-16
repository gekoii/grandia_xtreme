#include "common.h"
#include "field/event.h"
#include "field/n26.h"
#include "field/n27.h"
#include "protos/field.h"

s32 EvtScnrWorkLayout(s32);

s32 EvtScnrInit(s32 arg0) {
    s32 r;

    evtScnrWork = NULL;
    evtScnrRegSlots = NULL;
    evtScnrCur = NULL;
    evtMsgWins = NULL;
    evtMsgCtl = NULL;
    evtPictureReqs = NULL;
    evtScnrSndStatus = NULL;
    r = EvtScnrWorkLayout(arg0);
    evtScnrWork->vars = 0;
    evtScnrWork->sysVars = 0;
    EvtScnrWorkReset();
    return r;
}
