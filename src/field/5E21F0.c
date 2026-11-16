#include "common.h"
#include "field/state.h"

ChrObj *ChrTenantGet(u32 arg0) {
    if (arg0 >= chrTenantList.count) {
        return NULL;
    }
    return &chrTenantList.objs[arg0];
}
