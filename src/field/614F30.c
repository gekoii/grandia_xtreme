#include "common.h"
#include "field/n22.h"

u16 AmapFindEncounterKind(u16 id, FldMapSet *party) {
    s32 i;
    s32 r = -1;

    for (i = 0; i < 16; i++) {
        if ((s16)party->enc[i].kind == id) {
            r = party->enc[i].defNo - 1;
            break;
        }
    }
    return r;
}
