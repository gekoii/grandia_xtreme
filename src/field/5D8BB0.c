#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"
#include "field/n16.h"

extern void DbgChrMovePage(ChrObj *, u16);
extern s32 dbgFldPartyNo;
extern s32 dbgFldPartyPage;
extern u8 dbgPartyMenuStrTitle[];

void DbgFldPartyMenu(void) {
    Pad *pad = &padPort1;
    ChrObj *e;

    DbgPrintf(4, 3, dbgPartyMenuStrTitle, dbgFldPartyNo, 8, dbgFldPageColor5Str, dbgFldPartyPage + 1, dbgFldPageColor0Str);
    if ((pad->repeatFast & 0x8000) && dbgFldPartyNo != 0) {
        dbgFldPartyNo--;
    }
    if ((pad->repeatFast & 0x2000) && dbgFldPartyNo < 9) {
        dbgFldPartyNo++;
    }
    if ((pad->trigger & 4) && dbgFldPartyPage != 0) {
        dbgFldPartyPage--;
    }
    if ((pad->trigger & 8) && dbgFldPartyPage < 5) {
        dbgFldPartyPage++;
    }
    if (pad->trigger & 0x20) {
        ChrAllGatherToLeader();
    }
    e = ChrGetByNo(dbgFldPartyNo);
    switch (dbgFldPartyPage) {
    case 0:
        DbgChrStatusPage(e, pad->held);
        break;
    case 1:
        DbgChrFlagsPage(e, pad->held);
        break;
    case 2:
        DbgChrGroupPage(e, pad->held);
        break;
    case 3:
        DbgChrMotionPage(e, pad->held);
        break;
    case 4:
        DbgChrMovePage(e, pad->held);
        break;
    case 5:
        DbgChrFacePage(e, pad->held);
        break;
    }
}
