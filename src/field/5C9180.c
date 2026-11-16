#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

void MemSet(void *, s32, s32);

void TxtGlyphLayoutClear(s32 arg0) {
    if (EvtScnrHighIdLatch() != 1) {
        MemSet(&txtGlyphLayout[arg0], 0, 0x8010);
    }
}
