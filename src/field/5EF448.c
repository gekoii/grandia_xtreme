/* rodata: 0x00673D60-0x00673D78 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamSkillPage(s32 arg0, s32 sel, s32 mode, s32 dir, s32 page) {
    s32 i;
    s32 x;
    s32 j;
    s32 start = 0;
    s32 n = 45;

    switch (page) {
    case 0:
        start = 0;
        break;
    case 1:
        start = 45;
        break;
    case 2:
        start = 90;
        break;
    }
    x = mode * 4 - 2;
    DbgDrawCursor(dbgParamX + (sel / (n / 3)) * 16 + x, dbgParamY + sel % (n / 3));
    for (i = 0; i < 45 && i + start < 100; i++) {
        j = start + i;
        DbgPrintf(dbgParamX + (i / (n / 3)) * 16, dbgParamY + i % (n / 3), "%2d:%3d(%d:%d:%d)", j,
                      ptyPartyData->skills[start + i].id, ptyPartyData->skills[j].owner, ptyPartyData->skills[start + i].level, ptyPartyData->skills[start + i].exp);
    }
    switch (mode) {
    case 0:
        if (dir == 1) {
            ptyPartyData->skills[sel].id++;
        }
        if (dir == -1) {
            ptyPartyData->skills[sel].id--;
        }
        break;
    case 1:
        if (dir == 1) {
            ptyPartyData->skills[sel].level++;
        }
        if (dir == -1) {
            ptyPartyData->skills[sel].level--;
        }
        break;
    case 2:
        if (dir == 1) {
            ptyPartyData->skills[sel].exp++;
        }
        if (dir == -1) {
            ptyPartyData->skills[sel].exp--;
        }
        break;
    }
}
