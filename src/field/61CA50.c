#include "common.h"
#include "protos/field.h"
#include "field/state.h"

s32 FldSpotCountMonsters(void) {
    u32 i;
    s32 n;

    if (MapSetFileGet() == 0) {
        return 0;
    }
    n = 0;
    for (i = 0; i < fldSpotWork->count; i++) {
        if (fldSpotWork->list[i].kind == 2) {
            n++;
        }
    }
    return n;
}
