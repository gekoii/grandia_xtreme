#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

extern u16 evtScnrStartResult;

u16 EvtScnrStart(u16 id) {
    s32 reload;
    u16 flags;
    u8 *r;

    evtScnrStartResult = 2;
    reload = 0;
    if (EvtScnrFlagGet(evtScnrSysVarIds->restartFlag) != 0) {
        reload = 1;
    }
    flags = evtScnrWork->flags;
    if (flags & 0x200) {
        reload = 1;
    }
    if (evtScnrCur->id != 0xFFFF) {
        if ((evtScnrCur->id & 0xF000) != 0xE000) {
            reload = 1;
        } else if (evtScnrCur->id == 0xE000 && (id & 0xF000) == 0xE000) {
            if (flags & 0x100) {
                reload = 1;
            }
        } else if (evtScnrCur->id == id) {
            reload = 1;
        }
    }
    if (reload == 1) {
        if (evtScnrCur->unk9 == 0) {
            return evtScnrStartResult;
        }
        EvtScnrEnd();
    }
    if ((evtScnrCur->id & 0xF000) == 0xE000) {
        evtScnrCur->flags |= 0x40;
        EvtMsgWinCloseAll();
        evtScnrCur->id = 0xFFFF;
        evtScnrCur->pc = NULL;
        evtMsgCtl->state = 0xFF;
    }
    evtScnrCur->flags &= ~8;
    evtScnrStartResult = 1;
    r = EvtScnrFind(id);
    if (r == NULL) {
        evtScnrStartResult = 3;
    } else {
        EvtScnrScriptCtlReset();
        evtScnrCur->id = id;
        evtScnrCur->unk9 = 0;
        evtMsgCtl->flags = 0;
        evtScnrCur->pc = r;
        evtScnrStartResult = 0;
        if ((evtScnrCur->id & 0xF000) == 0xE000) {
            evtScnrCur->noticeTimer = 0x78;
        }
    }
    return evtScnrStartResult;
}
