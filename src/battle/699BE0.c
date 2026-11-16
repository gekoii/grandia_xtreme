#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void FxAtkWaveStart(BtlUnit *c, s8 arg1) {
    FxAtk *e;
    FxAtkWave *o;
    GphModel *s;
    BtlUnit *p;
    f32 v[2];
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 n;
    s32 x;
    s32 id;

    if (c->partsChr >= 0) {
        p = &btlUnits[c->partsChr];
    } else {
        p = c;
    }
    e = FxAtkAlloc(p, 1);
    if (e == NULL) {
        return;
    }
    o = MemAlloc16A(0x20);
    e->work = o;
    if (o == NULL) {
        FxAtkFree(e);
        return;
    }
    s = BtlModelCreateWithPool(fxAtkWaveModel, &e->pool);
    o->model = s;
    if (s == NULL) {
        FxAtkFree(e);
        return;
    }
    VuSinCosAngle(&v[0], &v[1], c->rot[1]);
    o->model->rot[1] = 0x10000 - c->rot[1];
    o->vx = v[0] * 40.0f;
    o->hitMask = 0;
    o->vz = v[1] * 40.0f;
    o->model->pos[0] = c->pos[0] + v[0] * 100.0f;
    o->model->pos[2] = c->pos[2] + v[1] * 100.0f;
    o->radius = p->scopeRadius;
    if (p->scopeType & 0x100) {
        for (j = 0; j < 4; j++) {
            o->hitMask |= 1 << j;
        }
    }
    if (p->scopeType & 0x200) {
        m = o->hitMask;
        for (k = 4; k < 16; k++) {
            m |= 1 << k;
        }
        o->hitMask = m;
    }
    o->hitMask &= ~(1 << p->id);
    o->delay = 0;
    o->state = 0;
    o->alpha = 1.0f;
    if (p->id >= 4) {
        id = (s32)BTL_UNIT_ACT(p)->level->effects;
    } else {
        id = 0;
    }
    e->update = FxAtkWaveUpdate;
    o->power = id;
    e->build = FxAtkWaveBuild;
    x = p->params->crtclMulti;
    if (x > 0) {
        n = x;
        if (n >= 4) {
            n = 3;
        }
        for (i = 8; n > 0; n--, i += 8) {
            FxAtkWaveSpawnEcho(p, e->work, i);
        }
    }
    if (arg1) {
        ((FxAtkWave *)e->work)->hitMask &= ~(1 << p->target);
    }
}
