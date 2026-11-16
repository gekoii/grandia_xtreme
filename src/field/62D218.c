#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);

void FldMenuTacticsInit(void) {
    s16 a;
    s16 b;
    s32 i;
    s32 id;

    MemSet(&fldMenuTactics, 0, sizeof(FldMenuTactics));
    FldMenuWinOpen(8, 0x180, 0xF0, 0x1E0, 0xF0, 0, 1, 0x88010);
    FldMenuWinSetActive(8);
    FldMenuGuideSet(4);
    for (i = 0; i < 4; i++) {
        id = sysSaveData.party[i];
        if (id != 0) {
            PtyCharGetTactics(--id, &a, &b);
            fldMenuTactics.tactics[i][0] = a;
            fldMenuTactics.tactics[i][1] = b;
        }
    }
    fldMenuTactics.cur = -1;
}
