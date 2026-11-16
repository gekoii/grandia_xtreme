#include "common.h"
#include "field/menu.h"
#include "field/n24.h"
#include "protos/field.h"


void MemSet(void *, s32, s32);

void FldMenuShopInit(s32 arg0) {
    s32 i;

    MemSet(&fldMenuShop, 0, 0x630);
    FldMenuWinOpen(10, 0x20, 0xE8, 4, 0x160, 0, 0, 0x9000);
    FldMenuWinOpen(8, 0x184, 0xCC, 0x1B8, 0xBE, 0, 1, 0x48010);
    FldMenuWinOpen(11, 0x1F4, 0x174, 0xD4, 0x5E, 0, 3, 0xF0);
    FldMenuWinOpen(12, 0x1C8, 0x60, 0xD4, 4, 0, 2, 8);
    fldMenuShop.nChr = ChrJoinedCount();
    for (i = 0; i < 8; i++) {
        ChrPortraitSetByCode(i, i + 0xFA1);
        fldMenuShop.scale[i] = 0;
        fldMenuShop.vel[i] = 0.1f - i * 0.01f;
        fldMenuShop.cnt[i] = 0;
    }
    fldMenuShop.last = -1;
    fldMenuShop.mode = arg0;
    if (fldMenuShop.mode == 0) {
        fldMenuShop.tab = 1;
    }
    FldMenuShopListBuild();
    fldMenuShop.state = 4;
    switch (fldMenuShop.mode) {
    case 0:
        FldMenuWinOpen(14, 0x140, 0xE0, 0xC8, 0x80, 0, 2, 0x80F0);
        FldMenuGuideSet(11);
        break;
    case 1:
        FldMenuWinOpen(14, 0x154, 0xE0, 0xF0, 0x80, 0, 2, 0x80F0);
        FldMenuGuideSet(11);
        break;
    case 2:
        FldMenuWinOpen(14, 0x14A, 0xF0, 0x104, 0x50, 0, 2, 0x80F0);
        FldMenuGuideSet(11);
        break;
    case 3:
        fldMenuShop.state = 2;
        FldMenuGuideSet(0x11);
        break;
    }
    FldMenuWinSetActive(14);
    if (fldMenuShop.mode != 2) {
        fldMenuShop.gold = itemWork->gold;
    } else {
        fldMenuShop.gold = PtyGetShopPoints();
    }
    SndSePlay(0x803, 0, 0, 0x1E, 0);
}
