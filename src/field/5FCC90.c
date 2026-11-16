#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n20.h"


void EvtMsgTextUpdate(void) {
    EvtMsgText t;

    if (evtScnrWork->winKind == 1) {
        t.layout = 0;
    } else {
        t.layout = 1;
    }
    if (evtMsgCtl->flags & 0x40) {
        t.layout = 1;
    }
    if (EvtMsgGetCtl()->name[0] != 0) {
        t.text = EvtMsgGetCtl()->name;
        if ((EvtScnrGetCur()->flags & 0x10) || (EvtMsgGetCtl()->flags & 0x3000)) {
            EvtMsgLayoutRun(&t, 0);
            EvtScnrGetCur()->flags &= 0xFFEF;
        } else if ((EvtMsgGetCtl()->flags & 0x20) || (EvtMsgGetCtl()->flags & 0x40)) {
            EvtMsgLayoutRun(&t, 1);
        } else {
            EvtMsgLayoutChars(&t, 1);
        }
        EvtMsgGlyphBufClear();
    }
}
