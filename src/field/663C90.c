/* rodata: 0x006761F0-0x00676228 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"

s32 EvtMsgCtlStart(void *, u8 *);

s32 EvtScnrMsgStart(void) {
    u8 buf[16];
    u8 *p;
    u8 *q;
    u8 c;

    p = evtScnrWork->pc;
    if (EvtScnrIsDebug()) {
        EvtDbgLog("scnr_message_start start");
    }
    evtMsgCtl->flags &= 0xF3FF;
    evtMsgCtl->unk20 = 2;
    evtMsgCtl->unk24 = 0;
    if (evtMsgCtl->flags & 0x20) {
        evtMsgCtl->unk34 = p;
        p--;
        evtMsgCtl->choiceCount = *p++ >> 5;
        evtScnrWork->unk449 = 0;
        EvtMsgCtlStart(buf, p);
        q = p;
        do {
            while (*q++ != 0x17) {
            }
        } while (*q++ != 0);
        p = q;
    } else {
        if (evtScnrWork->unk447 == 0) {
            EvtMsgWinPlace(evtScnrWork->winKind - 1, buf, 0, 0);
        } else {
            c = p[1];
            q = p + 2;
            EvtMsgWinPlace(evtScnrWork->unk447, buf, c >> 5, p[0] | ((c & 0x1F) << 8));
            p = q;
        }
        EvtMsgCtlStart(buf, p);
        q = p;
        evtScnrWork->unk449 = 0;
        for (;;) {
            if (*q++ == 0x17 && *q == 0) {
                p = q + 1;
                break;
            }
        }
        evtMsgCtl->flags &= 0xF4DF;
    }
    evtScnrWork->pc = p;
    if (EvtScnrIsDebug()) {
        EvtDbgLog("scnr_message_start end");
    }
    return 1;
}
