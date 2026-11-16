#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "pad.h"

void EvtScnrSysCmdExec(s32);

void EvtOccRequestUpdate(void) {
    OccsEntry *occs;
    OccEntry *occ;

    if (EvtScnrOccWakeGetKind() != -1) {
        if (EvtScnrOccWakeGetKind() == 0) {
            OccWakeById(EvtScnrOccWakeGetId());
        } else {
            OccsWakeById(EvtScnrOccWakeGetId());
        }
        EvtScnrOccWakeClear();
    }
    if (EvtScnrOccWaitGetState() == 1) {
        if (EvtScnrOccWaitGetKind() == 0) {
            occ = OccFindById(EvtScnrOccWaitGetId());
            if ((occ != NULL) && !(occ->flags & OCC_ACTIVE)) {
                EvtScnrOccWaitSet(2, 0, 0);
            }
        } else {
            occs = OccsFindById(EvtScnrOccWaitGetId());
            if ((occs != NULL) && (occs->state == 0)) {
                EvtScnrOccWaitSet(2, 0, 0);
            }
        }
    }
}

void EvtScnrPadUpdate(Pad *pad) {
    EvtScnrDebugOn();
    EvtScnrInputUpdate(pad);
}

void EvtScnrFrameUpdate(void) {
    s32 q;

    EvtMsgClearRequestA();
    EvtMsgClearRequestB();
    if ((evtMsgWins[0].flags & 4) || (evtMsgWins[1].flags & 4)) {
        EvtMsgTextUpdate();
        EvtMsgLineFeedUpdate();
    }
    EvtTenantPoseRequestFlush();
    EvtOccRequestUpdate();
    q = EvtScnrGetCmdQueue();
    if (q != 0) {
        EvtScnrSysCmdExec(q);
    }
}
