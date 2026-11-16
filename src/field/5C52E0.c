#include "common.h"
#include "field/n14.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void ChrTenantAreaAlloc(s32 n) {
    void *buf;
    s32 sz;

    chrTenantAreaTable = FldMemAlloc(8);
    MemSet(chrTenantAreaTable, 0, 8);
    chrTenantAreaTable->count = n;
    if (n != 0) {
        sz = n * 0x68;
        buf = FldMemAlloc(sz);
        MemSet(buf, 0, sz);
        if (buf != NULL) {
            chrTenantAreaTable->areas = buf;
        } else {
            chrTenantAreaTable->count = 0;
            chrTenantAreaTable->areas = NULL;
        }
    }
}
