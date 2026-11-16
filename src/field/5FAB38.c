#include "common.h"
#include "protos/field.h"


s32 ChrJoinedCount(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 8; i++) {
        if (EvtSystemFlagGet(i + 0xB) == 1) {
            n++;
        }
    }
    return n;
}
