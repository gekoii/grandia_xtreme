#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldCamScriptPage;
extern char dbgCamScrStrTitleRun[];
extern char dbgCamScrStrTitleStop[];

void DbgFldCamScriptMenu(void) {
    Pad *pad;
    u16 btn;

    pad = &padPort1;
    if (CamScrIsRunning() != 0) {
        DbgPrintf(4, 4, dbgCamScrStrTitleRun, dbgFldPageColor5Str, dbgFldCamScriptPage + 1, dbgFldPageColor0Str);
    } else {
        DbgPrintf(4, 4, dbgCamScrStrTitleStop, dbgFldPageColor5Str, dbgFldCamScriptPage + 1, dbgFldPageColor0Str);
    }
    if (!(pad->held & 2)) {
        btn = pad->trigger;
        if ((btn & 4) && dbgFldCamScriptPage != 0) {
            dbgFldCamScriptPage--;
        }
        if ((btn & 8) && dbgFldCamScriptPage < 3) {
            dbgFldCamScriptPage++;
        }
    }
    if (CamScrIsRunning() != 0 && dbgFldCamScriptPage == 0) {
        dbgFldCamScriptPage = 1;
    }
    switch (dbgFldCamScriptPage) {
    case 0:
        DbgFldCamScriptListPage();
        break;
    case 1:
        DbgFldCamScriptInfoPage();
        break;
    case 2:
        DbgFldCharaMovePage();
        break;
    case 3:
        DbgFldMainFlagPage();
        break;
    }
}
