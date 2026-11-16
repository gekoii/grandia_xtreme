#include "common.h"
#include "field/state.h"

void GphModelUpdateGsRegs(struct GphModel *);

void ChrTenantCalcPass(void) {
    u32 i;
    ChrObj *o;
    u16 f;

    for (i = 0; i < chrTenantList.count; i++) {
        o = &chrTenantList.objs[i];
        f = o->flags;
        if (f & 2) {
            if (f & 0x1000) {
                if (f & 1) {
                    if (o->model != 0) {
                        GphModelUpdateGsRegs(o->model);
                    }
                }
            }
        }
    }
}
