/* Port-only C for EvtScnrWorkLayout (0x006600C0), which the decompilation still has in assembly. */
#include "common.h"
#include "field/event.h"
#include "field/n26.h"
#include "field/n27.h"
#include "protos/field.h"

extern u32 evtScnrWorkEnd;

/* Carves the scenario work buffer into the scenario globals (field/event.h);
   returns the end of the buffer, work + 0x231C. */
s32 EvtScnrWorkLayout(s32 work) {
    u8 *w = (u8 *)work;

    evtScnrWorkEnd = (u32)(w + 0x1308);
    ((EvtScnrWork *)w)->sysVars = (s32)(w + 0x1308);
    evtScnrSysVarIds = (EvtScnrSysVarIds *)(w + 0x2308);
    evtScnrCur = (EvtScnrCur *)(w + 0x474);
    evtMsgWins = (EvtMsgWin *)(w + 0x5C4);
    evtScnrRegSlots = (EvtScnrRegSlot *)(w + 0x5FC);
    evtMsgCtl = (EvtMsgCtl *)(w + 0x488);
    evtScnrSndStatus = (EvtScnrSndStatus *)(w + 0x694);
    evtMsgCursor = (EvtMsgCursor *)(w + 0x770);
    evtScnrGameRefs = (EvtScnrGameRefs *)(w + 0x67C);
    evtScnrBlock790 = (EvtScnrBlock790 *)(w + 0x790);
    evtScnrInput = (u16 *)(w + 0x79E);
    evtScnrWork = (EvtScnrWork *)w;
    evtScnrWorkEnd = (u32)(w + 0x231C);
    evtPictureReqs = (EvtPictureReq *)(w + 0x7AE);
    evtScnrBlock7D8 = (s32 *)(w + 0x7D8);
    evtTenantPointReqs = (EvtTenantPointReq *)(w + 0x808);
    evtScnrCmdQueue = (EvtScnrCmd *)(w + 0xA08);
    return (s32)(w + 0x231C);
}
