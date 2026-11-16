#include "common.h"
#include "field/n23.h"

void FldMenuItemReset(void) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        fldMenuItem.marks[i] = 0;
    }
    fldMenuItem.cursor = 0;
    fldMenuItem.top = 0;
    fldMenuItem.unk4 = 0;
    fldMenuItem.unk8 = 0;
    fldMenuItem.unkC1 = 0;
    fldMenuItem.helpType = 0;
    fldMenuItem.helpId = 0;
    fldMenuItem.unkC4 = -1;
}
