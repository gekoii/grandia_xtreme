#include "common.h"
#include "field/event.h"

static inline void vset(f32 *d, f32 x, f32 y, f32 z) {
    d[0] = x;
    d[1] = y;
    d[2] = z;
}

void EvtAreaSetDefault(s32 arg0, EvtArea *arg1) {
    EvtArea *o = &evtAreaTable->areas[arg0];

    o->state = 1;
    o->id = 0;
    o->shape = EVT_AREA_BOX;
    o->mode = 0;
    o->unkA = 2;
    o->ofs[1] = 50.0f;
    o->link = NULL;
    o->chk = 0;
    o->ofs[0] = 100.0f;
    o->ofs[2] = 100.0f;
    o->radius = 86.6025f;
    vset(arg1->shapeData.box, 100.0f, 100.0f, 100.0f);
    arg1->result = 0;
}
