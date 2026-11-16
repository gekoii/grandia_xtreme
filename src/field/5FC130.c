#include "common.h"
#include "protos/field.h"


void EvtMsgLayoutClear(u8 arg0) {
    if (arg0 == 0xFF) {
        TxtGlyphLayoutClear(3);
        return;
    }
    TxtGlyphLayoutClear(arg0 + 3);
}
