#include "common.h"
#include "field/n24.h"
#include "protos/field.h"


void MemSet(void *, s32, s32);

void FldMenuJunctionInit(void) {
    FldMenuJunction *w = &fldMenuJunction;

    MemSet(w, 0, 0x60);
    FldMenuWinOpen(9, 0x20, 0xD4, 4, 0x160, 0, 0, 0x400000);
    FldMenuWinOpen(8, 0xC8, 0x70, 4, 4, 0, 3, 0x400000);
    FldMenuJunctionMeshInit();
    FldMenuJunctionScatterInit();
    w->unk34 = 0x40;
}
