/* rodata: 0x00675DF8-0x00675E40 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtMsgAnalyze(void) {
    u16 cmd;
    s32 n;
    u16 saved;
    s32 spd;
    u16 f;

    evtScnrMsgCmdPtr = evtScnrCmdQueue + evtScnrWork->cmdCount;
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_analysys start");
    }
    spd = 2;
    if (evtScnrWork->winKind == 1) {
        spd = 0;
    }
    spd += evtScnrCur->textSpeed;
    evtMsgCtl->unk14 = evtMsgCtl->cursorX;
    evtMsgCtl->unk16 = evtMsgCtl->cursorY;
    for (;;) {
        n = EvtMsgReadChar(&cmd);
        if (EvtScnrIsDebug() != 0) {
            if (cmd < 0x20) {
                EvtScnrDebugSetCmd(cmd, evtScnrWork->pc);
            } else {
                EvtScnrDebugSetCmd(cmd, 0);
            }
        }
        if (cmd < 0x20) {
            if (cmd != 0x17 && cmd != 0x1F && cmd != 0x1A) {
                if (evtMsgCtl->flags2 & 1) {
                    evtMsgCtl->flags2 |= 2;
                }
            }
            EvtMsgExecCode(cmd);
            evtMsgCtl->flags2 &= ~1;
        } else {
            evtMsgCtl->flags2 |= 1;
            saved = evtScnrWork->textColor;
            evtMsgCtl->unk0 = spd;
            if (evtMsgCtl->unk24 != 0 || (evtMsgCtl->flags & 0x3000)) {
                evtScnrCur->flags |= 0x10;
                evtScnrWork->pc -= n;
                evtScnrWork->pc = EvtMsgCopyRun(evtScnrWork->pc, 0);
            } else if (*(u32 *)evtMsgCtl & 0x600000) {
                evtScnrCur->flags |= 0x10;
                evtScnrWork->pc -= n;
                evtMsgCtl->flags &= ~8;
                evtScnrWork->pc = EvtMsgCopyRun(evtScnrWork->pc, 1);
                if (!(evtMsgCtl->flags & 0x20)) {
                    evtMsgCtl->flags |= 2;
                }
            } else {
                evtScnrWork->pc -= n;
                evtScnrWork->pc = EvtMsgCopyChar(evtScnrWork->pc);
            }
            evtScnrWork->textColor = saved;
            if (evtMsgCtl->unk24 == 0) {
                f = evtMsgCtl->flags;
                if (!(f & 0x3060)) {
                    if (!(f & 0x10)) {
                        evtMsgCtl->flags = f | 0x10;
                        evtMsgCtl->wait = evtMsgCtl->unk20;
                        goto done;
                    }
                }
            }
            if ((EVT_SCNR_FLAGS_WORD(evtScnrWork) & 0xC0) == 0xC0) {
                goto clear;
            }
        }
        f = evtMsgCtl->flags;
        if (f & 2) {
            evtMsgCtl->flags = f & ~2;
            goto done;
        }
        if (f & 4) {
            goto done;
        }
        if ((EVT_SCNR_FLAGS_WORD(evtScnrWork) & 0xC0) == 0xC0) {
            goto done;
        }
    }
done:
    if ((EVT_SCNR_FLAGS_WORD(evtScnrWork) & 0xC0) == 0xC0) {
    clear:
        evtScnrWork->flags &= ~0x40;
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_analysys end");
    }
}
