#include "common.h"
#include "protos/field.h"


s32 EvtFaceWinSetName(u16 *str, s32 y) {
    s32 i;

    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < 0x16 && str[i] != 0; i++) {
        TxtGlyphQueuePush(3, str[i], i, y);
    }
    return i;
}
