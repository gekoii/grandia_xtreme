#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"
#include "field/menu.h"


void FldMenuClose(void) {
    ChrObj *p;

    SndSePlay(0x7E0, 0, 0, 0x1E, 0);
    GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f);
    GphScreenFxSet(fldScreenFx, 2, 0, 0x14, 0.0f);
    fldMenuWork->screen = 0xFF;
    FldMenuWinCloseAll();
    FldMenuCursorReset();
    MenuHelpClose();
    p = ChrGetByNo(1);
    if (p != NULL) {
        FldHpGaugeInit(&p->hpGauge, sysSaveData.chars[0].hp, sysSaveData.chars[0].maxHp);
    }
}
