#include "common.h"
#include "field/state.h"
#include "field/n17.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void ChrTenantInit(void) {
    fldChrPackTable = FldMemAlloc(64 * sizeof(FldChrPack));
    MemSet(&chrTenantList, 0, sizeof(ChrTenantList));
    MemSet(fldChrPackTable, 0, 64 * sizeof(FldChrPack));
}
