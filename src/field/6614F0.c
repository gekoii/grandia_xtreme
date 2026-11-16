/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00675E40-0x00675EE8 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtMsgExecCode(u8 arg0) {
    u8 args[2];
    u16 w[3];
    s32 val[1];
    u8 *pc;
    u8 sub;
    EvtScnrSndStatus *e;

    pc = evtScnrWork->pc;
    evtScnrMsgCmdPtr->type = arg0;
    switch (arg0) {
    case 0:
    case 1:
    case 2:
        EvtMsgCodeEnd();
        evtMsgCtl->flags |= 4;
        break;
    case 4:
        pc = EvtMsgCodeSetIdBase(pc);
        break;
    case 5:
        args[0] = *pc++;
        args[1] = *pc++;
        switch (args[0]) {
        case 0:
            EvtScnrOccWakeRequest(evtScnrWork->idBase + args[1], 0);
            evtMsgCtl->flags |= 3;
            break;
        case 1:
            EvtScnrOccWakeRequest(evtScnrWork->idBase + args[1], 1);
            evtMsgCtl->flags |= 3;
            break;
        case 2:
        case 3:
            switch (EvtScnrOccWaitGetState()) {
            case -1:
                if (args[0] == 2) {
                    EvtScnrOccWaitSet(1, 0, evtScnrWork->idBase + args[1]);
                } else {
                    EvtScnrOccWaitSet(1, 1, evtScnrWork->idBase + args[1]);
                }
            case 1:
                pc -= 3;
                evtMsgCtl->flags |= 3;
                break;
            case 2:
                EvtScnrOccWaitClear();
                break;
            }
            break;
        }
        break;
    case 11:
        pc = EvtScnrOpFlag(pc);
        break;
    case 13:
        sub = *pc++;
        evtScnrMsgCmdPtr->args[0] = sub;
        switch (sub) {
        case 0:
            pc = EvtScnrReadU16(pc, w);
            evtScnrMsgCmdPtr->args[1] = w[0];
            pc = EvtScnrReadU16(pc, w);
            evtScnrMsgCmdPtr->args[2] = w[0];
            pc = EvtScnrReadU16(pc, w);
            evtScnrMsgCmdPtr->args[3] = w[0];
            pc = EvtScnrReadU16(pc, w);
            evtScnrMsgCmdPtr->args[4] = w[0];
            break;
        case 1:
            break;
        case 2:
        case 3:
            evtScnrMsgCmdPtr->args[1] = *pc++;
            break;
        case 4:
        case 5:
        case 6:
            break;
        }
        evtScnrMsgCmdPtr++;
        evtScnrWork->cmdCount++;
        break;
    case 19:
        args[0] = *pc++;
        pc = EvtScnrReadU32(pc, val);
        switch (args[0]) {
        case 0:
            EvtScnrMoneyToVar();
            break;
        case 1:
            EvtScnrMoneyAdd(val[0]);
            break;
        case 2:
            EvtScnrMoneySub(val[0]);
            break;
        }
        break;
    case 20:
        evtScnrMsgCmdPtr->args[0] = *pc++;
        evtScnrMsgCmdPtr->args[1] = *pc++;
        pc = EvtScnrReadU16(pc, w);
        evtScnrMsgCmdPtr->args[2] = w[0];
        evtScnrMsgCmdPtr->args[3] = *pc++;
        evtScnrMsgCmdPtr++;
        evtScnrWork->cmdCount++;
        break;
    case 21:
        evtScnrMsgCmdPtr->args[0] = *pc++;
        evtScnrMsgCmdPtr->args[1] = *pc++;
        evtScnrMsgCmdPtr->args[2] = *pc++;
        evtScnrMsgCmdPtr->args[3] = *pc++;
        evtScnrMsgCmdPtr++;
        evtScnrWork->cmdCount++;
        break;
    case 23:
        args[0] = *pc++;
        if (args[0] == 0) {
            EvtMsgCodeEnd();
        } else if (args[0] == 0x20) {
            pc = EvtMsgCodeWinSub(pc);
        } else if (args[0] == 0x40) {
            pc = EvtMsgCodeChoice(pc);
        } else if (args[0] == 0x80) {
            EvtMsgCodeWinNotice();
        } else if (evtScnrWork->lastWinCode != 0xFF) {
            EvtMsgCodeWinSelect();
        }
        evtScnrWork->lastWinCode = args[0];
        break;
    case 24:
        args[0] = *pc++;
        args[1] = *pc++;
        EvtScnrPictureRequest(args[1], args[0]);
        break;
    case 26:
        EvtMsgCodePageWait();
        break;
    case 27:
        EvtMsgCodeWaitButton();
        break;
    case 28:
        args[0] = *pc++;
        if (args[0] == 0) {
            evtMsgCtl->unk20 = 0;
            evtMsgCtl->unk24 |= 3;
            evtMsgCtl->flags = (evtMsgCtl->flags & ~0x10) | 1;
        } else {
            if (evtMsgCtl->unk24 == 3) {
                evtMsgCtl->unk24 = 0;
            }
            if (evtMsgCtl->unk24 == 0) {
                if (args[0] == 0xFF) {
                    evtMsgCtl->unk20 = 2;
                } else {
                    evtMsgCtl->unk20 = args[0];
                }
            }
        }
        break;
    case 29:
        args[0] = *pc++;
        if (args[0] == 0xFF) {
            args[1] = *pc++;
            if (evtMsgCtl->unk24 == 3) {
                evtMsgCtl->unk24 = 0;
            }
            if (evtMsgCtl->unk24 == 0) {
                evtMsgCtl->flags |= 0x12;
                evtMsgCtl->wait = args[1];
            }
        } else {
            e = evtScnrSndStatus;
            e += args[0];
            if (e->id == -1) {
                pc = EvtScnrReadU16(pc, w);
            } else {
                pc = EvtScnrReadU16(pc, w);
                if (evtScnrWork->flags & 1) {
                    if (!((f32)(e->time * 1.2) < w[0])) {
                        break;
                    }
                } else if (!(e->time < w[0])) {
                    break;
                }
                if (e->state != 0) {
                    pc -= 4;
                    evtMsgCtl->flags |= 0x12;
                }
            }
        }
        break;
    case 30:
        args[0] = *pc++;
        evtScnrWork->textColor = args[0];
        break;
    case 31:
        EvtMsgCodeNewLine();
        break;
    }
    evtScnrWork->pc = pc;
}
