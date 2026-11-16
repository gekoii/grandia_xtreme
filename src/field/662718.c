#include "common.h"
#include "field/event.h"
#include "field/n27.h"

extern u32 evtScnrStreamSyncStep;

void EvtScnrStreamSyncUpdate(void) {
    s32 found;
    s32 i;
    EvtScnrSndStatus *e;
    s32 m1;
    s32 one;

    found = 0;
    switch (evtScnrStreamSyncStep) {
    case 0:
        m1 = -1;
        one = 1;
        e = evtScnrSndStatus;
        for (i = 1; i >= 0; i--) {
            if (e->id != m1 && e->unk4 != m1 && e->unk10 != 0) {
                found = one;
            }
            e++;
        }
        if (found == 1) {
            evtScnrStreamSyncStep++;
        }
        break;
    case 1:
        evtScnrWork->streamWait = 1;
        evtScnrStreamSyncStep++;
        break;
    case 2:
        e = evtScnrSndStatus;
        if (e->id != -1 && e->state == 1) {
            break;
        }
        evtScnrStreamSyncStep++;
    case 3:
        evtScnrWork->streamWait = 1;
        evtScnrStreamSyncStep++;
        break;
    case 4:
        e = evtScnrSndStatus;
        e++;
        if (e->id == -1 || e->state != 1) {
            evtScnrStreamSyncStep = 0;
        }
        break;
    }
}
