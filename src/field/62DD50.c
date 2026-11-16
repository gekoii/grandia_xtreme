#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);

void FldMenuEquipInit(void) {
    MemSet(&fldMenuEquip, 0, sizeof(FldMenuEquip));
    FldMenuWinOpen(0xA, 0x82, 0xE6, 0xBE, 0xDC, 0, 8, 0x18);
    FldMenuWinOpen(8, 0x1AE, 0xF0, 0x168, 0xA8, 0, 8, 0x88000);
    FldMenuWinSetActive(8);
    FldMenuGuideSet(3);
    if (ChrPartyCount() >= 2) {
        MenuLrArrowsSet(320.0f, 224.0f, 320.0f);
    }
    fldMenuEquip.last = -1;
}
