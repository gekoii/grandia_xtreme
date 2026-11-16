/* rodata: 0x00673D78-0x00673DB0 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamEggPage(s32 arg0, s32 sel, s32 mode, s32 dir, s32 page) {
    s32 i;
    s32 base = 0;
    s32 n = 12;

    switch (page) {
    case 0:
        base = 0;
        break;
    case 1:
        base = 12;
        break;
    }
    DbgDrawCursor(dbgParamX - 2, dbgParamY + sel);
    for (i = 0; i < n; i++) {
        DbgPrintf(dbgParamX, dbgParamY + i, "%2d:%3d(%2d:%d:%2d/%2d)[%3d:%3d:%3d:%3d:%3d:%3d]", i,
                      ptyPartyData->eggs[i + base].itemId, ptyPartyData->eggs[i + base].owner, ptyPartyData->eggs[i + base].unk1A,
                      ptyPartyData->eggs[i + base].mp, ptyPartyData->eggs[i + base].maxMp,
                      ptyPartyData->eggs[i + base].spells[0], ptyPartyData->eggs[i + base].spells[1], ptyPartyData->eggs[i + base].spells[2],
                      ptyPartyData->eggs[i + base].spells[3], ptyPartyData->eggs[i + base].spells[4], ptyPartyData->eggs[i + base].spells[5]);
    }
    if (dir == 1) {
        ptyPartyData->eggs[sel].itemId++;
    }
    if (dir == -1) {
        ptyPartyData->eggs[sel].itemId--;
    }
}
