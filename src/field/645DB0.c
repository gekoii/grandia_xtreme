#include "common.h"
#include "field/n17.h"
#include "field/n24.h"


void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void MiniInit(void) {
    miniCtl = FldMemAlloc(0x70);
    MemSet(miniCtl, 0, 0x70);
    MemSet(mapExtraCharPacks, 0, 0xF0);
}
