#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtMsgWinUpdate(void) {
    s32 arr[2];
    s32 i;
    u16 f;
    EvtMsgCtl *c;

    if (EvtScnrFlagGet(0x8D8) != 0) {
        c = evtMsgCtl;
        f = c->flags;
        if (f & 8) {
            if (f & 4) {
                c->state = 0xFF;
                c->flags = 0;
                evtScnrWork->lastWinCode = 0xFF;
                EvtPictureReqsReset();
                return;
            }
            if (f & 0x100) {
                c->flags = f & 0xFEFF;
            }
            c->unk24 = 0;
            c->flags = (c->flags | 1) & 0x77F7;
        }
    } else {
        for (i = 0; i < 2; i++) {
            arr[i] = 0;
            if (evtMsgWins[i].layout != 0xFF) {
                if (evtMsgWins[i].flags & 1) {
                    evtMsgWins[i].flags &= 0xFE;
                    evtMsgWins[i].flags |= 0x84;
                }
                if (evtMsgWins[i].flags & 2) {
                    evtMsgWins[i].flags &= 0xFD;
                    evtMsgWins[i].flags |= 0x80;
                }
            }
        }
        EvtMsgWinTilesUpdate();
    }
}
