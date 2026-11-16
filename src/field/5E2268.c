#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrTenantAlloc(s32 arg0) {
    chrTenantList.count = arg0;
    chrTenantList.used = 0;
    chrTenantList.objs = ChrObjArrayAlloc(arg0);
}
