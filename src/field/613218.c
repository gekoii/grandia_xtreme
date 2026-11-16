#include "common.h"
#include "field/occ.h"

void AmapOccSetStateSet(OccEntry *obj, u8 op, u8 id, u8 value) {
    OccStateSet *s;

    s = &obj->params.stateSet;
    obj->kind = 0x1A;
    s->op = op;
    s->id = id;
    s->value = value;
    s->type = 5;
    s->unk4 = 0;
}
