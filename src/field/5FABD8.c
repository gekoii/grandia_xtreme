#include "common.h"
#include "field/n20.h"
#include "save.h"

s32 ChrPartySwapToSlot(s32 id, s32 slot) {
    s32 idx;
    s32 i;

    idx = -1;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] == id) {
            idx = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        chrPartyOrderBackup[i] = sysSaveData.party[i];
    }
    if (idx == -1) {
        return 0;
    }
    if (slot == idx) {
        return 1;
    }
    if (sysSaveData.party[idx] != sysSaveData.party[slot]) {
        sysSaveData.party[idx] ^= sysSaveData.party[slot];
        sysSaveData.party[slot] ^= sysSaveData.party[idx];
        sysSaveData.party[idx] ^= sysSaveData.party[slot];
    }
    return 1;
}
