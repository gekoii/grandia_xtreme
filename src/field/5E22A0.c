#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n17.h"


void ChrTenantSetupAll(ChrTenantDef *arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        ChrTenantSetup(&chrTenantList.objs[i], &arg0[i], i);
    }
    chrTenantList.used = arg1;
}
