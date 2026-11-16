#include "common.h"
#include "protos/field.h"
#include "field/state.h"


ChrObj *ChrTenantAdd(void *arg0) {
    ChrTenantList *m = &chrTenantList;
    ChrObj *o;

    if (m->used == m->count) {
        return NULL;
    }
    o = &m->objs[m->used];
    ChrTenantSetup(o, arg0, m->used);
    m->used++;
    return o;
}
