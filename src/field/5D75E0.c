#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n16.h"

extern s32 dbgFldScenarioMenuPage;
extern u8 dbgScnMenuStrTitle[];

void DbgFldScenarioMenu(void) {
    Pad *pad;
    u16 b;

    pad = &padPort1;
    DbgPrintf(2, 3, dbgScnMenuStrTitle, dbgFldPageColor5Str, dbgFldScenarioMenuPage + 1, dbgFldPageColor0Str, evtScnrCur->id);
    if (!(pad->held & 2)) {
        b = pad->trigger;
        if ((b & 4) && dbgFldScenarioMenuPage != 0) {
            dbgFldScenarioMenuPage--;
        }
        if (b & 8) {
            if (dbgFldScenarioMenuPage <= 0) {
                dbgFldScenarioMenuPage++;
            }
        }
    }
    switch (dbgFldScenarioMenuPage) {
    case 0:
        DbgFldScenarioInfoPage();
        break;
    case 1:
        DbgFldScenarioListPage();
        break;
    }
}
