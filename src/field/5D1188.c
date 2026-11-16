#include "common.h"
#include "field/n15.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void TxtMsgCursorInit(void) {
    TxtMsgCursor *p;

    p = FldMemAlloc(0x120);
    txtMsgCursorWork = p;
    MemSet(p, 0, 0x120);
    txtMsgCursorWork[0].u = 0;
}
