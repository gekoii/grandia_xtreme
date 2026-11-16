#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void TxtGlyphInit(void) {
    s32 i;

    txtGlyphLayout = FldMemAlloc(0x30060);
    txtGlyphQueue = FldMemAlloc(0x1830);
    MemSet(txtGlyphLayout, 0, 0x30060);
    MemSet(txtGlyphQueue, 0, 0x1830);
    for (i = 0; i < 6; i++) {
        TxtGlyphLayoutClear(i);
        TxtGlyphQueueRequestClear(i);
    }
}
