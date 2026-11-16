#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"


void FxAtkRingStart(BtlUnit *c, s8 arg1, BtlUnit *tgt, s32 flags, f32 radius, s32 arg5) {
    FxAtk *e;
    FxAtkRing *o;
    GphModel *s;
    s32 i;
    s32 n;
    s32 j;
    s32 m;
    s32 k;
    s32 x;
    BtlUnit *p;

    if (c->partsChr >= 0) {
        p = &btlUnits[c->partsChr];
    } else {
        p = c;
    }
    e = FxAtkAlloc(p, 5);
    if (e == NULL) {
        return;
    }
    o = MemAlloc16A(0x1C);
    e->work = o;
    if (o == NULL) {
        FxAtkFree(e);
        return;
    }
    s = BtlModelCreateWithPool(fxAtkRingModel, &e->pool);
    o->model = s;
    if (s == NULL) {
        FxAtkFree(e);
        return;
    }
    o->hitMask = 0;
    s->pos[0] = tgt->pos[0];
    s->pos[2] = tgt->pos[2];
    o->radius = radius;
    if (flags & 0x100) {
        for (j = 0; j < 4; j++) {
            o->hitMask |= 1 << j;
        }
    }
    if (flags & 0x200) {
        m = o->hitMask;
        for (k = 4; k < 16; k++) {
            m |= 1 << k;
        }
        o->hitMask = m;
    }
    o->hitMask &= ~(1 << p->id);
    o->delay = 0;
    o->alpha = 1.0f;
    e->update = FxAtkRingUpdate;
    e->build = FxAtkRingBuild;
    o->state = 0;
    o->t = 0.0f;
    o->power = arg5;
    x = p->params->crtclMulti;
    if (x > 0) {
        n = x;
        if (n >= 4) {
            n = 3;
        }
        for (i = 8; n > 0; n--, i += 8) {
            FxAtkRingSpawnEcho(p, e->work, i);
        }
    }
    if (arg1) {
        ((FxAtkRing *)e->work)->hitMask &= ~(1 << tgt->id);
    }
}
