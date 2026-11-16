#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"

typedef struct {
    u8 pad0[0x20];
} Buf;


void FxAtkMultiHitStart(BtlUnit *c, s32 arg1, s32 arg2) {
    Buf buf;
    FxAtk *e;
    FxAtkMultiHit *o;
    s32 i;
    void *t;
    BtlUnit *p;

    if (c->partsChr >= 0) {
        p = &btlUnits[c->partsChr];
    } else {
        p = c;
    }
    e = FxAtkAlloc(p, 4);
    if (e == NULL) {
        return;
    }
    o = MemAlloc16A(0x24);
    e->work = o;
    if (o == NULL) {
        FxAtkFree(e);
        return;
    }
    o->count = (u8)c->params->crtclMulti;
    if (o->count >= 4) {
        o->count = 3;
    }
    BtlModelPoolReserve(fxAtkMultiHitModel, o->count);
    e->pool = BtlModelPoolAlloc(&buf);
    if (e->pool == 0) {
        FxAtkFree(e);
        return;
    }
    for (i = 0; i < o->count; i++) {
        t = BtlModelCreate(fxAtkMultiHitModel, 0);
        o->models[i] = t;
        o->alpha[i] = 0.0f;
    }
    for (; i < 3; i++) {
        o->models[i] = NULL;
        o->alpha[i] = 0.0f;
    }
    ChrModelPoolSet(0);
    o->state = 0;
    o->timer = 8;
    o->next = 0;
    o->target = (BtlUnit *)arg1;
    o->power = arg2;
    e->update = FxAtkMultiHitUpdate;
    e->build = FxAtkMultiHitBuild;
}
