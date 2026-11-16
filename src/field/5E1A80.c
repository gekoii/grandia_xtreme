#include "common.h"
#include "field/state.h"

s32 ChrTenantGetCount(void) {
    return chrTenantList.count;
}

ChrObj *ChrTenantAt(s32 arg0) {
    return &chrTenantList.objs[arg0];
}
