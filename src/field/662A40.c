#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtScnrRun(s32 arg0) {
    evtScnrWork->unk446 = (evtScnrCur->flags >> 1) & 1;
    evtScnrWork->unk44A = (evtScnrCur->flags >> 2) & 1;
    evtScnrWork->unk449 = evtScnrCur->unk5;
    evtScnrWork->textColor = evtScnrCur->unk6;
    evtScnrWork->pc = evtScnrCur->pc;
    evtScnrWork->unk447 = evtScnrCur->unk9;
    evtScnrWork->winKind = evtScnrCur->winKind;
    if (evtScnrWork->unk446 != 0) {
        evtScnrCur->id = 0xFFFF;
        evtScnrCur->pc = NULL;
        return;
    }
    EvtScnrExec();
    if ((evtScnrCur->flags >> 1) & 1) {
        evtScnrWork->unk446 = 1;
        if (evtScnrCur->flags & 1) {
            evtScnrCur->id = 0xFFFF;
        }
    } else {
        if (arg0 != 0) {
            evtScnrCur->flags &= 0xF9;
        } else {
            evtScnrCur->flags &= 0xF8;
        }
        evtScnrCur->flags |= evtScnrWork->unk446 << 1;
        evtScnrCur->flags |= evtScnrWork->unk44A << 2;
        evtScnrCur->unk6 = evtScnrWork->textColor;
        evtScnrCur->winKind = evtScnrWork->winKind;
        evtScnrCur->unk5 = evtScnrWork->unk449;
        evtScnrCur->pc = evtScnrWork->pc;
    }
}
