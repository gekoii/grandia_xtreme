#include "common.h"
#include "save.h"



s32 ChrIsInParty(u8 id) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] == id) {
            return 1;
        }
    }
    return 0;
}
