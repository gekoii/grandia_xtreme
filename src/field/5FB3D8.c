#include "common.h"
#include "save.h"



s32 ChrPartyCountForBattle(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] != 0) {
            n++;
        }
    }
    return n;
}
