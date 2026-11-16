#include "common.h"
#include "field/n14.h"

void TxtGlyphQueueRequestClear(u8 id) {
    txtGlyphQueue[id].clearReq = 1;
}
