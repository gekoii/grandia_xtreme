#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n12.h"


void ChrTenantBuildDrawList(void) {
    u32 i;
    ChrObj *o;
    u16 f;

    GphDrawListSet(fldTenantDrawList);
    GphDrawListClear(NULL);
    for (i = 0; i < chrTenantList.count; i++) {
        o = &chrTenantList.objs[i];
        f = o->flags;
        if (f & 2) {
            if (f & 0x1000) {
                if (f & 1) {
                    if (!(f & 0x10)) {
                        if (EvtSystemFlagGet(0x72) == 1) {
                            continue;
                        }
                        if (!(o->flags & 0x10)) {
                            goto release; /* goto: one flag 0x73 test for both paths, as the original */
                        }
                    }
                    if (EvtSystemFlagGet(0x73) == 1) {
                        continue;
                    }
                release:
                    if (o->model != 0) {
                        GphModelCalcAndDraw(o->model);
                    }
                }
            }
        }
    }
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
}
