#include "common.h"
#include "save.h"



s32 FldMenuPartyNextSlot(u8 arg0) {
    s32 i;

    if (arg0 >= 3) {
        return 0;
    }
    for (i = arg0 + 1; i < 4; i++) {
        if (sysSaveData.party[i] != 0) {
            return i;
        }
    }
    return 0;
}
