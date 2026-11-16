#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n23.h"

void FldMenuTacticsApply(void) {
    s32 i;
    s32 id;

    for (i = 0; i < 4; i++) {
        id = sysSaveData.party[i];
        if (id != 0) {
            PtyCharSetTactics(--id, fldMenuTactics.tactics[i][0], fldMenuTactics.tactics[i][1]);
        }
    }
}
