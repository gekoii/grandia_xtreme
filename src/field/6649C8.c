#include "common.h"
#include "field/event.h"

void *memset(void *, s32, u32);

void EvtMsgGlyphBufClear(void) {
    memset(evtMsgCtl->name, 0, 0x100);
}
