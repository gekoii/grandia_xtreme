/* rodata: 0x00673D40-0x00673D50 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamItemPage(s32 arg0, s32 sel, s32 mode, s32 dir) {
    s32 i;
    s32 x;

    x = mode * 9 - 2;
    DbgDrawCursor(dbgParamX + (sel / 15) * 25 + x, dbgParamY + sel % 15);
    for (i = 0; i < 30; i++) {
        DbgPrintf(dbgParamX + (i / 15) * 25, dbgParamY + i % 15, "%2d:%3dx%3d", i, ptyPartyData->bag[i].id, ptyPartyData->bag[i].count);
    }
    if (dir == 1) {
        if (mode == 0) {
            ptyPartyData->bag[sel].id++;
        } else if (ptyPartyData->bag[sel].id != 0 && ptyPartyData->bag[sel].count < 10) {
            ptyPartyData->bag[sel].count++;
        }
    }
    if (dir == -1) {
        if (mode == 0) {
            ptyPartyData->bag[sel].id--;
        } else if (ptyPartyData->bag[sel].id != 0 && ptyPartyData->bag[sel].count > 0) {
            ptyPartyData->bag[sel].count--;
        }
    }
}

void DbgParamValuablePage(s32 arg0, s32 sel, s32 mode, s32 dir) {
    s32 i;
    s32 x;

    x = mode * 9 - 2;
    DbgDrawCursor(dbgParamX + (sel / 12) * 25 + x, dbgParamY + sel % 12);
    for (i = 0; i < 24; i++) {
        DbgPrintf(dbgParamX + (i / 12) * 25, dbgParamY + i % 12, "%2d:%3dx%3d", i, ptyPartyData->keyItems[i].id, ptyPartyData->keyItems[i].count);
    }
    if (dir == 1) {
        if (mode == 0) {
            ptyPartyData->keyItems[sel].id++;
        } else if (ptyPartyData->keyItems[sel].id != 0 && ptyPartyData->keyItems[sel].count < 10) {
            ptyPartyData->keyItems[sel].count++;
        }
    }
    if (dir == -1) {
        if (mode == 0) {
            ptyPartyData->keyItems[sel].id--;
        } else if (ptyPartyData->keyItems[sel].id != 0 && ptyPartyData->keyItems[sel].count > 0) {
            ptyPartyData->keyItems[sel].count--;
        }
    }
}
