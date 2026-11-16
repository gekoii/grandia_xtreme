#include "common.h"
#include "protos/field.h"
#include "field/state.h"


ChrObj *ChrTenantAddFromPack(void *arg0) {
    ChrTenantList *m = &chrTenantList;
    ChrObj *o;

    if (m->used == m->count) {
        return NULL;
    }
    o = &m->objs[m->used];
    ChrTenantSetupFromPack(o, arg0, m->used);
    m->used++;
    return o;
}
