#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "save.h"


void ChrNonPartyDeactivate(void) {
    s32 i;
    ChrObj *c;

    for (i = 1; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c != NULL && sysSaveData.party[0] != i && sysSaveData.party[1] != i &&
            sysSaveData.party[2] != i && sysSaveData.party[3] != i && (c->flags & 1)) {
            c->flags ^= 1;
        }
    }
}
