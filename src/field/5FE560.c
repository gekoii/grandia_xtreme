#include "common.h"
#include "field/state.h"
#include "protos/field.h"

ChrObj *EvtCharSelFindTenant(u16 id) {
    s32 i;
    ChrObj *e;

    for (i = 0; i < ChrTenantGetCount(); i++) {
        e = ChrTenantGet(i);
        if (e == NULL) {
            return NULL;
        }
        if (e->chrId == id) {
            return e;
        }
    }
    return NULL;
}
