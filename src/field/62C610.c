#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);

void FldMenuPartyOrderInit(void) {
    s16 ids[4];
    s32 i;
    s32 lv;

    MemSet(&fldMenuPartyOrder, 0, sizeof(FldMenuPartyOrder));
    FldMenuWinOpen(8, 0x212, 0xE6, 0xB4, 0x190, 0, 1, 0x8018);
    fldMenuPartyOrder.count = ChrJoinedCount() - 1;
    PtyPartyGet(ids);
    fldMenuPartyOrder.slots[0] = ids[0];
    fldMenuPartyOrder.slots[1] = ids[1];
    fldMenuPartyOrder.slots[2] = ids[2];
    fldMenuPartyOrder.slots[3] = ids[3];
    for (i = 0; i < 4; i++) {
        lv = fldMenuPartyOrder.slots[i];
        if (lv != 0) {
            lv--;
            FldMenuWinOpen(i + 9, i * 0xC + 0xDC, i * 0x5A + 0x70, 0x15E, 0x60, 0, 0, 0x108010);
            FldMenuWinSetPortrait(i + 9, i, (lv + 1) * 100, 0, 0);
        } else {
            FldMenuWinOpen(i + 9, i * 0xC + 0xDC, i * 0x5A + 0x70, 0x15E, 0x60, 0, 0, 0x8010);
        }
    }
    FldMenuPartyOrderSlotsBuild();
    FldMenuGuideSet(0xA);
}
