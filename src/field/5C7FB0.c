#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

s32 ChrMotHas(ChrObj *o, u8 id) {
    ChrMotTable *p;

    if (o == NULL || o->model == NULL || (p = o->motTable) == NULL) {
        return 0;
    }
    if (id >= 64) {
        return 0;
    }
    return p->slots[id] != 0xFFFFFFFF;
}
