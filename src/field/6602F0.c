#include "common.h"
#include "field/event.h"


void EvtMsgCtlReset(void) {
    evtMsgCtl->state = 0xFF;
    evtMsgCtl->flags = 0;
    evtMsgCtl->flags2 = 0;
    evtMsgCtl->cursorX = 0;
    evtMsgCtl->cursorY = 0;
    evtMsgCtl->unkC = 0;
    evtMsgCtl->unkE = 0;
    evtMsgCtl->unk10 = 0;
    evtMsgCtl->unk12 = 0;
    evtMsgCtl->unk14 = 0;
    evtMsgCtl->unk16 = 0;
    evtMsgCtl->choice = 0;
    evtMsgCtl->choiceCount = 0;
    evtMsgCtl->wait = 0;
    evtMsgCtl->unk20 = 2;
    evtMsgCtl->unk22 = 2;
    evtMsgCtl->unk24 = 0;
    evtMsgCtl->textPc = 0;
    evtMsgCtl->ret[0] = 0;
    evtMsgCtl->ret[1] = 0;
    evtMsgCtl->name[0] = 0;
    evtMsgCtl->unk34 = 0;
}
