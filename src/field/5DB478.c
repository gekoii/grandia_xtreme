#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldOccMenuPage;
extern char dbgOccMenuStrTitle[];

void DbgFldOccMenu(void) {
    Pad *pad;
    u16 btn;

    pad = &padPort1;
    DbgPrintf(2, 2, dbgOccMenuStrTitle, dbgFldPageColor5Str, dbgFldOccMenuPage + 1, dbgFldPageColor0Str);
    if (!(pad->held & 2)) {
        btn = pad->trigger;
        if ((btn & 4) && dbgFldOccMenuPage != 0) {
            dbgFldOccMenuPage--;
        }
        if ((btn & 8) && dbgFldOccMenuPage <= 0) {
            dbgFldOccMenuPage++;
        }
    }
    switch (dbgFldOccMenuPage) {
    case 0:
        DbgFldOccPage();
        break;
    case 1:
        DbgFldOccsPage();
        break;
    }
}
