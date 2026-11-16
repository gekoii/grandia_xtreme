#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

typedef struct {
    u16 id;
    u16 pad;
} ItemDef;

extern ItemDef fldFlagListEntries[];

void FldFlagListOpen(u16 arg0) {
    s32 i;

    fldFlagListWork->count = 0;
    for (i = 0; i < 0x26; i++) {
        if (EvtGlobalFlagGet(fldFlagListEntries[i].id) == 1) {
            fldFlagListWork->entries[fldFlagListWork->count] = i;
            fldFlagListWork->count++;
        }
    }
    if (fldFlagListWork->count == 0) {
        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
        return;
    }
    fldFlagListWork->pages = fldFlagListWork->count / 6;
    if (fldFlagListWork->count % 6 != 0) {
        fldFlagListWork->pages = fldFlagListWork->pages + 1;
    }
    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
    fldFlagListWork->open = 1;
    fldFlagListWork->event = arg0;
    fldFlagListWork->cursor = 0;
    fldFlagListWork->page = 0;
    fldFlagListWork->unk6 = 0;
    FldMenuWinOpen(2, 0x140, 0xE0, 0x190, 0xAA, 0, 0, 0x8881);
    FldMenuWinSetLayer(2, 0, txtWinTextures, fldFlagListWork->winSprites, 128.0f);
    FldMenuWinSetActive(2);
    FldFlagListSetPage(fldFlagListWork->page);
    if (fldFlagListWork->count >= 7) {
        FldMenuWinLrArrowsSet(2, 1, 12.0f);
    }
}
