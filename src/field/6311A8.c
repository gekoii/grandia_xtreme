#include "common.h"
#include "save.h"



s32 FldMenuPartyPrevSlot(u8 arg0) {
    s32 i;
    s32 r;

    r = 0;
    if (arg0 == 0) {
        for (i = 0; i < 4; i++) {
            if (sysSaveData.party[i] != 0) {
                r = i;
            }
        }
    } else {
        for (i = 0; i < arg0; i++) {
            if (sysSaveData.party[i] != 0) {
                r = i;
            }
        }
    }
    return r;
}
