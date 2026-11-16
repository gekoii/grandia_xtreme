#include "common.h"
#include "grxsys.h"
#include "field/state.h"

extern s32 chrInstSysData;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void ChrInstInit(void) {
    ChrInstPool **p = &chrInstPool;
    void *t = FldMemAlloc(sizeof(ChrInstPool));
    *p = t;
    MemSet(t, 0, sizeof(ChrInstPool));
    chrInstSysData = sysGrxsysPac->modelPackB0;
}
