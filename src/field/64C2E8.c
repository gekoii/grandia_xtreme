#include "common.h"
#include "field/n25.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void MiniUiAlloc(void) {
    miniLayout = FldMemAlloc(0x24);
    miniUi = FldMemAlloc(0x4D0);
    MemSet(miniLayout, 0, 0x24);
    MemSet(miniUi, 0, 0x4D0);
}
