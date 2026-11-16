/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtScnrStep(void) {
    EvtScnrWork *s;
    s32 v;
    EvtScnrSndStatus *e;

    s = evtScnrWork;
    if (s->waitTimer > 0.0f) {
        s->waitTimer = s->waitTimer - ((s->flags & 1) ? 1.2f : 1.0f);
        return;
    }
    if (s->streamWait == 1) {
        e = &evtScnrSndStatus[s->streamIndex];
        v = e->state;
        if (v == 1) {
            return;
        }
        if (v == 2) {
            s->streamWait = 0;
        }
    }
    if (evtScnrCur->pc != 0) {
        if (evtScnrCur->unk5 != 0xFF) {
            evtScnrWork->unk449 = evtMsgCtl->state;
            if (evtScnrWork->unk449 != 0xFF) {
                return;
            }
            evtScnrCur->unk5 = evtScnrWork->unk449;
            EvtMsgWinCloseAll();
            EvtPictureReqsReset();
        }
        if (evtScnrCur->flags & 1) {
            EvtScnrRun(0);
        } else {
            EvtScnrRun(1);
        }
    } else {
        evtScnrCur->flags &= ~8;
    }
}
