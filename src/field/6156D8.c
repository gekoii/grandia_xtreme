#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void AmapInit(void) {
    amapState = FldMemAlloc(0x5C0);
    amapCellModels = FldMemAlloc(0x80);
    amapObjModels = FldMemAlloc(0x400);
    MemSet(amapState, 0, 0x5C0);
    MemSet(amapCellModels, 0, 0x80);
    MemSet(amapObjModels, 0, 0x400);
    amapCellInsts = 0;
    AmapGenReset(&amapWork);
    amapWork.tiles = FldMemAlloc(AmapGenGetBufSize(&amapWork));
    amapWork.cells = FldMemAlloc(AmapGenGetCellBufSize(&amapWork));
}
