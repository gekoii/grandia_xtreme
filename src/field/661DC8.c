#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"


u8 *EvtMsgCodeWinSub(u8 *p) {
    s32 out[4];
    u16 f;
    u8 lo;
    u8 hi;

    if (evtScnrCur->winKind != 2) {
        f = evtMsgCtl->flags;
        if (f & 0x200) {
            evtMsgCtl->cursorX = 0;
            evtMsgCtl->cursorY = 0;
            lo = *p++;
            hi = *p++;
            EvtMsgWinPlace(3, out, hi >> 5, lo | ((hi & 0x1F) << 8));
            evtScnrCur->flags |= 0x40;
            evtMsgCtl->unkC = out[0];
            evtMsgCtl->unkE = out[1];
            evtMsgCtl->flags = (evtMsgCtl->flags | 0x40) & ~0x201;
            evtMsgCtl->unk34 = p;
        } else {
            p -= 2;
            evtMsgCtl->flags = f | 0x20A;
            EvtMsgCodeWaitButton();
            evtMsgCtl->unk24 = 0;
            *evtScnrInput &= 0x9FFF;
        }
        if (!(evtMsgCtl->flags & 0x800)) {
            EvtMsgPromptShow(1, 1);
        }
    }
    return p;
}
