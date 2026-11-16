/* rodata: 0x00675D40-0x00675DF8 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

typedef struct {
    u16 a;
    u8 pad2[2];
    u16 b;
    u8 pad6[10];
} Res;

typedef union {
    u32 w;
    u16 h[2];
} W32;

void EvtMsgMain(void) {
    Res res;

    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main start");
    }
    if (evtMsgCtl->state == 0xFF) {
        return;
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main quit");
    }
    if (evtMsgCtl->unk24 == 0 && evtScnrWork->streamWait != 1 && !(*(u32 *)evtMsgCtl & 0x02680000)
        && EvtScnrFlagGet(evtScnrSysVarIds->autoFlag) == 0 && EvtScnrFlagGet(evtScnrSysVarIds->noSkipFlag) == 0) {
        u16 v = *evtScnrInput;
        if (v & 0x4000) {
            EvtMsgCtl *c = evtMsgCtl;
            *evtScnrInput = v & 0xBFFF;
            c->unk24 |= 1;
            evtMsgCtl->wait = 2.0f;
            evtMsgCtl->flags = (evtMsgCtl->flags | 0x10) & 0xFFFE;
            return;
        }
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main button");
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main wait");
    }
    {
        EvtMsgCtl *ctl = evtMsgCtl;
        if (ctl->flags & 0x10) {
            if (ctl->wait > 0.0) {
                if (evtScnrWork->flags & 1) {
                    ctl->wait -= 1.2f;
                } else {
                    ctl->wait -= 1.0f;
                }
            } else {
                ctl->wait = 0.0f;
            }
            if (evtMsgCtl->wait == 0.0f) {
                evtMsgCtl->flags = (evtMsgCtl->flags & 0xFFEF) | 1;
            }
        }
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main mess disp");
    }
    {
        EvtMsgCtl *ctl = evtMsgCtl;
        u16 f = ctl->flags;
        if (f & 1) {
            s32 n = ctl->textPc;
            ctl->flags = f & 0xFFFE;
            evtScnrWork->pc = n;
            EvtMsgAnalyze();
            evtMsgCtl->textPc = evtScnrWork->pc;
        }
    }
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_message_main window");
    }
    {
        EvtMsgCtl *ctl = evtMsgCtl;
        u16 f = ctl->flags;
        if (f & 0x20) {
            if (f & 8) {
                switch (*evtScnrInput) {
                case 1:
                    if (ctl->choice != 0) {
                        ctl->choice = ctl->choice - 1;
                        evtScnrCur->flags |= 0x40;
                        EvtMsgChoiceRedraw();
                    }
                    break;
                case 2:
                    if (ctl->choice < ctl->choiceCount - 1) {
                        ctl->choice = ctl->choice + 1;
                        evtScnrCur->flags |= 0x40;
                        EvtMsgChoiceRedraw();
                    }
                    break;
                case 0x2000:
                    EvtScnrVarSet(2, evtScnrSysVarIds->resultVar, ctl->choice + 1);
                    {
                        u16 g = evtMsgCtl->flags;
                        if (g & 4) {
                            evtMsgCtl->flags = 0;
                            EvtMsgTileRectClear(evtMsgWins[1].x0, evtMsgWins[1].y0, evtMsgWins[1].x1, evtMsgWins[1].y1);
                            evtMsgCtl->state = 0xFF;
                        } else {
                            evtMsgCtl->flags = (g & 0xFFD7) | 1;
                            evtMsgCtl->unkC = 1;
                            evtMsgCtl->unkE = 0x13;
                            evtMsgCtl->cursorX = 0;
                            evtMsgCtl->cursorY = 0;
                            evtMsgCtl->choiceCount = 0;
                        }
                    }
                    EvtMsgWinCloseSlot(1);
                    evtScnrCur->flags |= 0x40;
                    evtMsgCtl->unk24 = 0;
                    break;
                }
            }
        } else if (f & 0x40) {
            if (!(f & 0x10)) {
                if (f & 8) {
                    if ((f & 0xC00) || (*evtScnrInput & 0x6000)) {
                        if (f & 4) {
                            ctl->state = 0xFF;
                            ctl->flags = 0;
                            evtScnrCur->flags |= 0x60;
                        } else {
                            ctl->flags = (f & 0xF7B7) | 1;
                            evtScnrCur->flags |= 0x60;
                            EvtMsgWinCloseSlot(0);
                            EvtMsgWinCloseSlot(1);
                            {
                                u16 g = evtMsgCtl->flags;
                                if (g & 0x100) {
                                    evtMsgCtl->flags = g & 0xFEFF;
                                }
                            }
                            EvtMsgWinPlace(evtScnrWork->lastWinCode - 1, &res, 3, 0);
                            evtMsgCtl->unkC = res.a;
                            evtMsgCtl->unkE = res.b;
                            evtMsgCtl->cursorX = 0;
                            evtMsgCtl->cursorY = 0;
                        }
                        evtMsgCtl->unk24 = 0;
                    }
                } else {
                    ctl->flags = f | 1;
                }
            }
        } else if (evtScnrCur->noticeTimer != 0) {
            evtScnrCur->noticeTimer--;
            if (evtScnrCur->noticeTimer <= 0) {
                evtScnrCur->noticeTimer = 0;
                EvtScnrEnd();
            }
        } else if ((f & 8) && !(evtScnrCur->flags & 0x10)) {
            if (EvtScnrIsDebug() != 0) {
                char buf[32] = "btn wait";
                EvtDbgLog(buf);
            }
            if ((evtMsgCtl->flags & 0xC00) || EvtScnrFlagGet(evtScnrSysVarIds->autoFlag) != 0 || (*evtScnrInput & 0x6000)) {
                EvtMsgCtl *c = evtMsgCtl;
                u32 g;
                u32 h;
                if ((g = c->flags) & 4) {
                    c->state = 0xFF;
                    c->flags = 0;
                    if (evtScnrWork->winKind == 1) {
                        evtScnrCur->flags |= 0x20;
                    } else {
                        evtScnrCur->flags |= 0x40;
                    }
                    evtScnrWork->winKind = 0xFF;
                } else {
                    if (g & 0x100) {
                        h = g & 0xFEFF;
                        c->flags = h;
                        EvtMsgWinCloseSlot(evtScnrWork->winKind);
                        EvtMsgWinPlace(evtScnrWork->winKind - 1, &res, 3, 0);
                        c = evtMsgCtl;
                    }
                    c->flags = (c->flags & 0xF7F7) | 1;
                    if (!(((W32 *)c)->w & 0x82000000)) {
                        if (evtScnrWork->winKind == 1) {
                            evtScnrCur->flags |= 0x20;
                        } else {
                            evtScnrCur->flags |= 0x40;
                        }
                    }
                    c = evtMsgCtl;
                    c->unk24 = 0;
                    h = c->flags & 0x7FFF;
                    c->flags = h;
                }
            }
        }
    }
}
