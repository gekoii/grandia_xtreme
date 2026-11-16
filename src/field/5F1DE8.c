#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "save.h"

void MemSet(void *, s32, s32);

void PtyPartyClear(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (ptyPartyUnits[i].id != 0) {
            PtyCharStoreUnitStats(&ptyPartyUnits[i]);
            sysSaveData.party[i] = 0;
            MemSet(&ptyPartyUnits[i], 0, sizeof(PtyUnit));
        }
    }
}
