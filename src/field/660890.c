#include "common.h"
#include "protos/field.h"

extern char evtScnrStrMainStart[];
extern char evtScnrStrMsgMainEnd[];
extern char evtScnrStrMainEnd[];
extern char evtScnrStrDispStart[];
extern char evtScnrStrDispEnd[];

void EvtScnrMainRoutine(void) {
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog(evtScnrStrMainStart);
    }
    EvtScnrStep();
    EvtMsgMain();
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog(evtScnrStrMsgMainEnd);
    }
    EvtMsgWinUpdate();
    EvtScnrStreamSyncUpdate();
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog(evtScnrStrMainEnd);
    }
}

void EvtScnrDispRoutine(void) {
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog(evtScnrStrDispStart);
    }
    EvtMsgDispUpdate();
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog(evtScnrStrDispEnd);
    }
}
