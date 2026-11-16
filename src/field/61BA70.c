#include "common.h"
#include "field/state.h"

s32 FldSpotIsIdle(void) {
    u32 i;

    for (i = 0; i < fldSpotWork->count; i++) {
        if (fldSpotWork->list[i].unkB != 0) {
            return 0;
        }
    }
    return 1;
}
