#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtMsgCodeEnd(void) {
    EvtScnrCur *u;
    EvtMsgCtl *c;
    u16 f;

    u = evtScnrCur;
    c = evtMsgCtl;
    u->flags &= ~8;
    f = c->flags | 0xC;
    c->flags = f;
    if (f & 0x20) {
        u->flags |= 0x60;
        return;
    }
    if (!(*(u32 *)c & 0x30000000)) {
        if (evtScnrWork->winKind == 2) {
            c->unkE = 0x13;
            EvtMsgPromptShow(1, 0);
        } else {
            c->unkE = 1;
            EvtMsgPromptShow(1, 1);
        }
    }
    if (EvtScnrFlagGet(evtScnrSysVarIds->autoFlag) != 0) {
        evtScnrCur->flags |= 0x60;
    }
    evtMsgCtl->unk24 = 0;
}
