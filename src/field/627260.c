#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

extern u8 fldMenuItemGuideModes;

void *MemSet(void *, s32, s32);

void FldMenuItemInit(void) {
    MemSet(&fldMenuItem, 0, sizeof(FldMenuItem));
    FldMenuItemReset();
    FldMenuItemListWinOpen(8, 8);
    FldMenuGuideSet(fldMenuItemGuideModes);
    FldMenuWinOpen(0xA, 0x1CC, 0x190, 0x12A, 0x50, 0, 1, 0x890);
    FldMenuItemCmdTextsInit();
}
