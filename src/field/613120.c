#include "common.h"
#include "field/occ.h"

void AmapOccSetSound(OccEntry *obj, s16 se) {
    OccSound *s;

    obj->kind = 0x10;
    s = &obj->params.sound;
    s->se = se;
    s->unk12 = 100;
    s->mode = 0;
}

void AmapOccSetChrFlag(OccEntry *obj, s16 id, s16 op, s16 clear) {
    OccChrFlag *s;

    s = &obj->params.chrFlag;
    obj->kind = 0x12;
    s->clear = clear;
    s->id = id;
    s->op = op;
    s->aiMode = 0;
    s->target = 0;
}
