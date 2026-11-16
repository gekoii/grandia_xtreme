#include "common.h"
#include "field/state.h"

ChrObj *ChrTenantFindById(s32 arg0) {
    u32 i; ChrObj *o; u32 n;
    for (i = 0; i < chrTenantList.count; i++) { o = &chrTenantList.objs[i]; if (o->id == arg0) return o; }
    return NULL;
}
