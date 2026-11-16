#include "common.h"
#include "chr.h"
#include "field/state.h"
#include "field/n12.h"
#include "field/n13.h"
#include "field/n17.h"

void MemSet(void *, s32, s32);
extern u8 fldInstancePool[];

void FldGlobalsReset(void) {
    MemSet(&camCtl, 0, 0x1D0);
    MemSet(camField, 0, 0x300);
    MemSet(&fldModelTable, 0, 0x404);
    MemSet(fldInstancePool, 0, 0x14);
    MemSet(&chrMotionList, 0, 0x404);
    MemSet(fldPartyPackTable, 0, 0x18C);
    MemSet(fldScreenFx, 0, 0xB0);
    fldPacketList = 0;
}
