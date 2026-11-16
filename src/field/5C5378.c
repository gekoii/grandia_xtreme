#include "common.h"
#include "field/n14.h"

ChrTenantArea *ChrTenantAreaGet(s32 arg0) {
    if ((arg0 < 0) || (arg0 >= chrTenantAreaTable->count)) {
        return NULL;
    }
    return &chrTenantAreaTable->areas[arg0];
}
