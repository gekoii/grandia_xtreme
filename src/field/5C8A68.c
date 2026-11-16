#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n14.h"

/* TxtMsgWinClose takes u8 but the caller passes i unmasked (s32 cast); the
   two final calls go through s64-returning casts so they are not tail calls. */
typedef s64 (*FnA)(s32, f32, f32);
typedef s64 (*FnB)(void);

void TxtMsgWinUpdate(void) {
    s32 i;
    u16 new_var;
    EvtMsgWin *s;

    for (i = 0; i < 2; i++) {
        s = (EvtMsgWin *)(i * 0x1C + (u32)evtMsgWins);
        if (s->layout != 0xFF) {
            if (!(s->flags & 8)) {
                s->flags ^= 8;
                new_var = (u16)s->y1;
                TxtMsgWinOpen(i, s->layout, (u16)s->x0 * 16, (u16)s->y0 * 16, ((u16)s->x1 - (u16)s->x0) * 16,
                              (new_var - (u16)s->y0) * 16, evtMsgWins[i].layout);
                txtMsgWinOpened[i * 2 + 1] = 1;
            }
        } else if (EvtScnrHighIdLatch() == 0) {
            ((void (*)(s32))TxtMsgWinClose)(i);
        }
    }
    if (evtMsgWins[0].layout != 0xFF || evtMsgWins[1].layout != 0xFF) {
        if (evtMsgCtl->flags & 0x20) {
            ((FnA)FldMenuCursorSetAt)(1, 4.0f, (f32)(evtMsgCtl->choice * 24) + 22.0f);
        } else {
            ((FnB)FldMenuCursorReset)();
        }
    }
}
