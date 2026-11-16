#include "common.h"
#include "field/n22.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void FldPointLightInit(void) {
    fldPointLights = FldMemAlloc(0xC00);
    fldPointLightSort = FldMemAlloc(0x100);
    MemSet(fldPointLights, 0, 4);
    MemSet(fldPointLightSort, 0, 4);
    fldPointLightCount = 0;
}
