#include "common.h"
#include "protos/field.h"
#include "party.h"


void PtyPartyStoreStats(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (ptyPartyUnits[i].id != 0) {
            PtyCharStoreUnitStats(&ptyPartyUnits[i]);
        }
    }
}
