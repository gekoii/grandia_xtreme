#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"
#include "field/n16.h"

extern void DbgChrMovePage(ChrObj *, u16);
extern u32 dbgFldTenantNo;
extern s32 dbgFldTenantPage;
extern u8 dbgTenantMenuStrTitle[];

void DbgFldTenantMenu(ChrTenantList *list) {
    Pad *pad = &padPort1;

    if (list == NULL) {
        return;
    }
    DbgPrintf(4, 3, dbgTenantMenuStrTitle, dbgFldTenantNo, list->count, dbgFldPageColor5Str, dbgFldTenantPage + 1, dbgFldPageColor0Str);
    if (list->count == 0) {
        return;
    }
    if (dbgFldTenantPage != 4) {
        if ((pad->repeatFast & 0x8000) && dbgFldTenantNo != 0) {
            dbgFldTenantNo--;
        }
        if ((pad->repeatFast & 0x2000) && dbgFldTenantNo < list->count - 1) {
            dbgFldTenantNo++;
        }
    }
    if ((pad->trigger & 4) && dbgFldTenantPage != 0) {
        dbgFldTenantPage--;
    }
    if ((pad->trigger & 8) && dbgFldTenantPage < 5) {
        dbgFldTenantPage++;
    }
    camCtl.target = ChrTenantAt(dbgFldTenantNo);
    camCtl.aiType = 1;
    switch (dbgFldTenantPage) {
    case 0:
        DbgChrStatusPage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    case 1:
        DbgChrFlagsPage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    case 2:
        DbgChrGroupPage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    case 3:
        DbgChrMotionPage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    case 4:
        DbgChrMovePage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    case 5:
        DbgChrFacePage(&list->objs[dbgFldTenantNo], pad->held);
        break;
    }
}
