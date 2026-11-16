#include "common.h"

typedef struct {
    u8 n;
    u8 pad;
    u16 id[8];
} Ent;

extern Ent fldMenuItemCmdTexts[5];

void FldMenuItemCmdTextsInit(void) {
    fldMenuItemCmdTexts[0].n = 0x3;
    fldMenuItemCmdTexts[0].id[0] = 0x3A4;
    fldMenuItemCmdTexts[0].id[1] = 0x3A6;
    fldMenuItemCmdTexts[0].id[2] = 0x3A8;
    fldMenuItemCmdTexts[0].id[3] = 0x3AA;
    fldMenuItemCmdTexts[0].id[4] = 0x3AC;
    fldMenuItemCmdTexts[0].id[5] = 0x3AE;
    fldMenuItemCmdTexts[0].id[6] = 0x3B0;
    fldMenuItemCmdTexts[0].id[7] = 0x3B2;
    fldMenuItemCmdTexts[1].n = 0x4;
    fldMenuItemCmdTexts[1].id[0] = 0x3AA;
    fldMenuItemCmdTexts[1].id[1] = 0x3AC;
    fldMenuItemCmdTexts[1].id[2] = 0x3AE;
    fldMenuItemCmdTexts[1].id[3] = 0x3B0;
    fldMenuItemCmdTexts[1].id[4] = 0x3B2;
    fldMenuItemCmdTexts[1].id[5] = 0x3B4;
    fldMenuItemCmdTexts[1].id[6] = 0x3B6;
    fldMenuItemCmdTexts[1].id[7] = 0x3B8;
    fldMenuItemCmdTexts[2].n = 0x5;
    fldMenuItemCmdTexts[2].id[0] = 0x3B2;
    fldMenuItemCmdTexts[2].id[1] = 0x3B4;
    fldMenuItemCmdTexts[2].id[2] = 0x3B6;
    fldMenuItemCmdTexts[2].id[3] = 0x3B8;
    fldMenuItemCmdTexts[2].id[4] = 0x3BA;
    fldMenuItemCmdTexts[2].id[5] = 0x3BC;
    fldMenuItemCmdTexts[2].id[6] = 0x3BE;
    fldMenuItemCmdTexts[2].id[7] = 0x3C0;
    fldMenuItemCmdTexts[3].n = 0x6;
    fldMenuItemCmdTexts[3].id[0] = 0x398;
    fldMenuItemCmdTexts[3].id[1] = 0x39A;
    fldMenuItemCmdTexts[3].id[2] = 0x39C;
    fldMenuItemCmdTexts[3].id[3] = 0x39E;
    fldMenuItemCmdTexts[3].id[4] = 0x3A0;
    fldMenuItemCmdTexts[3].id[5] = 0x3A2;
    fldMenuItemCmdTexts[3].id[6] = 0x3A4;
    fldMenuItemCmdTexts[3].id[7] = 0x3A6;
    fldMenuItemCmdTexts[4].n = 0x8;
    fldMenuItemCmdTexts[4].id[0] = 0x3BC;
    fldMenuItemCmdTexts[4].id[1] = 0x3BE;
    fldMenuItemCmdTexts[4].id[2] = 0x3C0;
    fldMenuItemCmdTexts[4].id[3] = 0x3C2;
    fldMenuItemCmdTexts[4].id[4] = 0x3C4;
    fldMenuItemCmdTexts[4].id[5] = 0x3C6;
    fldMenuItemCmdTexts[4].id[6] = 0x3C8;
    fldMenuItemCmdTexts[4].id[7] = 0x3CA;
}
