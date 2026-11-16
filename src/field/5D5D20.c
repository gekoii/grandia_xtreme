/* rodata: 0x006715A8-0x006715C0 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "save.h"



extern s32 dbgFldPartyMemberLine;
extern char dbgFldPartyNames[][9];

void DbgFldPartyMemberPage(void) {
    s32 i;
    Pad *pad = &padPort1;

    DbgDrawCursor(3, dbgFldPartyMemberLine + 6);
    for (i = 0; i < 4; i++) {
        DbgPrintf(4, i + 6, "%d  ID %4x [%s] ", i, sysSaveData.party[i], dbgFldPartyNames[sysSaveData.party[i]]);
    }
    if (!(pad->held & 2)) {
        if ((pad->repeatFast & 0x1000) && dbgFldPartyMemberLine != 0) {
            dbgFldPartyMemberLine--;
        }
        if ((pad->repeatFast & 0x4000) && dbgFldPartyMemberLine < 3) {
            dbgFldPartyMemberLine++;
        }
        if ((pad->trigger & 0x8000) && sysSaveData.party[dbgFldPartyMemberLine] != 0 &&
            (dbgFldPartyMemberLine != 0 || sysSaveData.party[0] != 1)) {
            ChrPartyMoveTo(ChrGetByPartySlot(1));
            sysSaveData.party[dbgFldPartyMemberLine]--;
            ChrLeaderOnlyActivate();
        }
        if ((pad->trigger & 0x2000) && sysSaveData.party[dbgFldPartyMemberLine] < 8) {
            ChrPartyMoveTo(ChrGetByPartySlot(1));
            sysSaveData.party[dbgFldPartyMemberLine]++;
            ChrLeaderOnlyActivate();
        }
    }
}
