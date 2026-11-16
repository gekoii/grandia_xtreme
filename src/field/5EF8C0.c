/* rodata: 0x00673DB0-0x00673DC8 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamSpecialPage(s32 arg0, s32 sel, s32 mode, s32 dir, s32 page, s32 init) {
    s32 i;
    s32 x;
    s32 start;
    s32 n;

    if (page != 1) {
        n = 40;
        start = 0;
    } else {
        n = 50;
        start = 40;
    }
    x = mode * 9 - 2;
    DbgDrawCursor(dbgParamX + (sel / (n / 3)) * 16 + x, dbgParamY + sel % (n / 3));
    for (i = 0; i < n; i++) {
        DbgPrintf(dbgParamX + (i / (n / 3)) * 16, dbgParamY + i % (n / 3), "%2d:%3d lv%2d(%d/%d)", i,
                      ptyPartyData->moveLevels[i + start].moveId, ptyPartyData->moveLevels[i + start].exp, ptyPartyData->moveLevels[i + start].level,
                      ptyPartyData->moveLevels[i + start].maxLevel);
    }
    switch (mode) {
    case 0:
        if (dir == 1) {
            ptyPartyData->moveLevels[sel + start].moveId++;
        }
        if (dir == -1) {
            ptyPartyData->moveLevels[sel + start].moveId--;
        }
        break;
    case 1:
        if (dir == 1) {
            ptyPartyData->moveLevels[sel + start].exp++;
        }
        if (dir == -1) {
            ptyPartyData->moveLevels[sel + start].exp--;
        }
        break;
    case 2:
        if (dir == 1) {
            ptyPartyData->moveLevels[sel + start].level++;
        }
        if (dir == -1) {
            ptyPartyData->moveLevels[sel + start].level--;
        }
        break;
    }
    if (init != 0) {
        for (i = 0; i < 90; i++) {
            if (ptyPartyData->moveLevels[i].level == 0) {
                ptyPartyData->moveLevels[i].level = 1;
            }
            if (ptyPartyData->moveLevels[i].exp == 0) {
                ptyPartyData->moveLevels[i].exp = 1;
            }
        }
    }
}
