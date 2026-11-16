/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00675F90-0x006760A0 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void *memset(void *, s32, u32);

/* Runs the field script until a command ends the frame (stop), the command
 * queue fills up, or the script ends; queued commands go to evtScnrCmdQueue. */
void EvtScnrExec(void) {
    u8 b[4];
    u16 s[2];
    s32 w[2];
    u8 *p;
    EvtScnrCmd *cmd;
    s32 stop;
    u8 c;
    u8 *q;
    EvtScnrWork *st;
    EvtTenantPointReq *e6;
    EvtScnrSndStatus *e;

    stop = 0;
    p = evtScnrWork->pc;
    evtScnrCur->condResult = 1;
    memset(evtScnrCmdQueue, 0, 0x900);
    cmd = evtScnrCmdQueue;
    evtScnrWork->cmdCount = 0;
    do {
        c = *p++;
        cmd->type = c;
        switch (c) {
        case 0:
        case 1:
        case 2:
            EvtScnrEnd();
            stop = 1;
            break;
        case 3:
            b[0] = *p;
            c = (b[0] & 0xF0) >> 4;
            if (c < 4) {
                q = p;
                p = EvtScnrOpJump(q);
                if (!(q < p)) {
                    stop = 1;
                }
            } else if (c == 4) {
                p = EvtScnrReadU16(p + 1, &s[0]);
                b[1] = s[0] >> 13;
                s[1] = s[0] & 0x1FFF;
                if (b[1] != 0) {
                    if (EvtScnrFlagGet(s[1]) == 0) {
                        p -= 4;
                        stop = 1;
                        break;
                    }
                } else {
                    if (EvtScnrFlagGet(s[1]) != 0) {
                        p -= 4;
                        stop = 1;
                        break;
                    }
                }
            } else if (c == 5) {
                p = EvtScnrReadU16(p + 1, &s[0]);
                s[1] = (f32)(rand() >> 16) * (1.0f / 32768.0f) * (f32)s[0];
                EvtScnrVarSet(2, evtScnrSysVarIds->resultVar, s[1]);
            }
            break;
        case 4:
            evtScnrWork->idBase = *p++ << 8;
            break;
        case 5:
            b[0] = *p++;
            b[1] = *p++;
            switch (b[0]) {
            case 0:
                EvtScnrOccWakeRequest(evtScnrWork->idBase + b[1], 0);
                stop = 1;
                break;
            case 1:
                EvtScnrOccWakeRequest(evtScnrWork->idBase + b[1], 1);
                stop = 1;
                break;
            case 2:
            case 3:
                switch (EvtScnrOccWaitGetState()) {
                case -1:
                    if (b[0] == 2) {
                        EvtScnrOccWaitSet(1, 0, evtScnrWork->idBase + b[1]);
                    } else {
                        EvtScnrOccWaitSet(1, 1, evtScnrWork->idBase + b[1]);
                    }
                    p -= 3;
                    stop = 1;
                    break;
                case 1:
                    p -= 3;
                    stop = 1;
                    break;
                case 2:
                    EvtScnrOccWaitClear();
                    break;
                }
                break;
            }
            break;
        case 6:
            e6 = &evtTenantPointReqs[evtScnrWork->unk46C];
            p = EvtScnrReadU16(p, &e6->tenant);
            e6->point = *p++;
            evtScnrWork->unk46C++;
            break;
        case 7:
            c = *p++;
            cmd->args[0] = c;
            switch (c) {
            case 5:
                cmd->args[1] = *p++;
                cmd->args[2] = *p;
                p += 3;
                break;
            case 0:
            case 1:
            case 2:
            case 4:
            case 6:
                p += 4;
                break;
            case 11:
                cmd->args[1] = *p++;
                cmd->args[2] = *p++;
                cmd->args[3] = *p++;
                cmd->args[4] = *p++;
                cmd->args[5] = *p++;
                cmd->args[6] = *p++;
                break;
            default:
                cmd->args[1] = *p++;
                cmd->args[2] = *p++;
                cmd->args[3] = *p++;
                cmd->args[4] = *p++;
                break;
            }
            cmd++;
            stop = 1;
            evtScnrWork->cmdCount++;
            break;
        case 8:
            p = EvtScnrOpVar(p, 0);
            break;
        case 9:
            p = EvtScnrOpVar(p, 1);
            break;
        case 10:
            p = EvtScnrOpVar(p, 2);
            break;
        case 11:
            p = EvtScnrOpFlag(p);
            break;
        case 12:
            cmd->args[0] = *p++;
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 13:
            c = *p++;
            cmd->args[0] = c;
            switch (c) {
            case 0:
                p = EvtScnrReadU16(p, &s[0]);
                cmd->args[1] = s[0];
                p = EvtScnrReadU16(p, &s[0]);
                cmd->args[2] = s[0];
                p = EvtScnrReadU16(p, &s[0]);
                cmd->args[3] = s[0];
                p = EvtScnrReadU16(p, &s[0]);
                cmd->args[4] = s[0];
                break;
            case 2:
            case 3:
                cmd->args[1] = *p++;
                break;
            case 1:
            case 4:
            case 5:
            case 6:
                break;
            }
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 14:
            c = *p++;
            cmd->args[0] = c;
            switch (cmd->args[0]) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                p += 2;
                break;
            case 5:
                p = EvtScnrReadU16(p, (u16 *)&cmd->args[1]);
                break;
            case 6:
                cmd->args[1] = *p;
                p = EvtScnrReadU16(p + 1, (u16 *)&cmd->args[2]);
                break;
            }
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 15:
            b[0] = *p++;
            cmd->args[0] = b[3] >> 5;
            cmd->args[1] = (b[3] & 0x18) >> 3;
            cmd->args[2] = b[3] & 7;
            if (cmd->args[0] < 8) {
                p = EvtScnrReadU16(p, (u16 *)&cmd->args[3]);
            }
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 16:
            cmd->args[0] = *p;
            stop = 1;
            p = EvtScnrReadU16(p + 1, (u16 *)&cmd->args[1]);
            p = EvtScnrReadU16(p, (u16 *)&cmd->args[2]);
            p = EvtScnrReadU16(p, (u16 *)&cmd->args[3]);
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 17:
            b[0] = *p;
            p = EvtScnrReadU16(p + 1, &s[0]);
            switch (b[0]) {
            case 0:
                EvtScnrItemTestToVar(s[0]);
                break;
            case 1:
                EvtScnrItemInc(s[0]);
                break;
            case 2:
                break;
            case 3:
                EvtScnrItemDec(s[0]);
                break;
            }
            break;
        case 18:
            b[0] = *p++;
            break;
        case 19:
            b[0] = *p;
            p = EvtScnrReadU32(p + 1, w);
            switch (b[0]) {
            case 0:
                EvtScnrMoneyToVar();
                break;
            case 1:
                EvtScnrMoneyAdd(w[0]);
                break;
            case 2:
                EvtScnrMoneySub(w[0]);
                break;
            }
            break;
        case 20:
            cmd->args[0] = *p++;
            cmd->args[1] = *p++;
            p = EvtScnrReadU16(p, &s[0]);
            cmd->args[2] = s[0];
            cmd->args[3] = *p++;
            switch (cmd->args[1]) {
            case 0:
                break;
            case 1:
                stop = 1;
                break;
            case 2:
                stop = 1;
                break;
            case 3:
                stop = 1;
                break;
            case 4:
                stop = 1;
                break;
            }
            cmd++;
            evtScnrWork->cmdCount++;
            break;
        case 21:
            st = evtScnrWork;
            cmd->args[0] = *p++;
            cmd->args[1] = *p++;
            cmd->args[2] = *p++;
            cmd->args[3] = *p++;
            cmd++;
            st->cmdCount++;
            break;
        case 24:
            b[0] = *p++;
            b[1] = *p++;
            EvtScnrPictureRequest(b[1], b[0]);
            break;
        case 29:
            b[0] = *p++;
            if (b[0] == 0xFF) {
                evtScnrWork->waitTimer = *p++;
            } else {
                e = evtScnrSndStatus;
                e += b[0];
                if (e->id == -1) {
                    p = EvtScnrReadU16(p, &s[0]);
                    break;
                }
                p = EvtScnrReadU16(p, &s[0]);
                if (evtScnrWork->flags & 1) {
                    if ((f32)(e->time * 1.2) < (f32)s[0]) {
                        if (e->state != 0) {
                            p -= 4;
                        }
                    }
                } else {
                    if (e->time < s[0]) {
                        if (e->state != 0) {
                            p -= 4;
                        }
                    }
                }
            }
            stop = 1;
            break;
        case 23:
            b[0] = *p;
            evtScnrCur->flags |= 0x1A0;
            p++;
            if (b[0] != 0x20) {
                EvtMsgTileGridClear();
                evtScnrCur->flags |= 0x20;
            }
            if (b[0] == 0) {
                EvtMsgCodeEnd();
                break;
            } else if (b[0] != 0x20) {
                if (b[0] == 0x40) {
                    p = EvtMsgCodeChoice(p);
                } else if (b[0] == 0x80) {
                    EvtMsgCodeWinNotice();
                } else if (evtScnrWork->winKind != 0xFF) {
                    EvtMsgCodeWinSelect();
                }
                if (b[0] != 0x20) {
                    evtScnrWork->winKind = b[0];
                }
            }
        default:
            evtScnrWork->pc = p;
            evtScnrCur->flags |= 8;
            EvtScnrMsgStart();
            *evtScnrInput = 0;
            return;
        }
        st = evtScnrWork;
        if (st->unk446 != 0) {
            goto end;
        }
        /* flags read as a word (lw): the u16 field gives lhu */
        if ((*(s32 *)&st->flags & 0xC0) == 0xC0) {
            st->pc = p;
            st->flags &= ~0x40;
            goto end;
        }
        if (stop) {
            st->pc = p;
            goto end;
        }
    } while (st->cmdCount < 0x40);
    {
        char buf[32] = "scnr command over";
        EvtDbgLog(buf);
    }
end:
    evtScnrWork->pc = p;
}
