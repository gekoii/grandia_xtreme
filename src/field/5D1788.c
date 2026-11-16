#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void FldMapCtlInit(void) {
    u16 v;

    MapLoadStateInit();
    MemSet(&mapNo, 0, 0xA);
    MapLoadStateIdle();
    FldMemInit(0xCDFF00);
    FldGlobalsReset();
    FldWorkAlloc();
    v = sysSaveData.mapNo;
    mapNo.cur = v;
    mapNo.file = v;
    mapNo.prev = v;
}
