#include "common.h"
#include "protos/field.h"


s32 PtyPartyCheckChars(u16 *ids) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (PtyPartyFindChar(ids[i] - 1) == -1) {
            return 0;
        }
    }
    for (i = 0; i < 4; i++) {
    }
    return 0;
}
