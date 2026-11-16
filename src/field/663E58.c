#include "common.h"
#include "protos/field.h"
#include "field/event.h"

u8 *EvtScnrOpJump(u8 *p) {
    s16 v;
    u8 b = *p;

    p = EvtScnrReadU16(p + 1, &v);
    switch (b >> 4) {
    case 0:
        if (evtMsgCtl->ret[1] != NULL) {
            p = evtMsgCtl->ret[1];
            evtMsgCtl->ret[1] = NULL;
        } else {
            p = evtMsgCtl->ret[0];
            evtMsgCtl->ret[0] = NULL;
        }
        break;
    case 1:
        if (evtScnrCur->condResult == 0) {
            p = evtScnrWork->body + (((b & 0xF) << 16) + (u16)v);
            evtScnrCur->condResult = 1;
        }
        break;
    case 3:
        if (evtMsgCtl->ret[0] == NULL) {
            evtMsgCtl->ret[0] = p;
        } else {
            evtMsgCtl->ret[1] = p;
        }
        /* fallthrough */
    case 2:
        p = evtScnrWork->body + (((b & 0xF) << 16) + (u16)v);
        break;
    }
    return p;
}
