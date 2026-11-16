/* rodata: 0x00673D50-0x00673D60 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamBookPage(s32 arg0, s32 sel, s32 mode, s32 dir) {
    s32 i;
    s32 x;
    s32 n = 24;
    s32 by;

    x = mode * 9 - 2;
    by = dbgParamY;
    DbgDrawCursor(dbgParamX + (sel / (n / 2)) * 25 + x, by + sel % (n / 2));
    for (i = 0; i < 24 && i < 100; i++) {
        DbgPrintf(dbgParamX + (i / (n / 2)) * 25, dbgParamY + i % (n / 2), "%2d:%3d(%d)", i, ptyPartyData->books[i].itemId, ptyPartyData->books[i].owner);
    }
    if (dir == 1) {
        ptyPartyData->books[sel].itemId++;
    }
    if (dir == -1) {
        ptyPartyData->books[sel].itemId--;
    }
}
