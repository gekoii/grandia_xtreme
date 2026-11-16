#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"

/* accesses through one-member unions (alias set 0) keep the original load/store order */
typedef union { s16 v; } U16;
typedef union { s32 v; } U32;
#define UNION_S16(x) (((U16 *)&(x))->v)
#define UNION_S32(x) (((U32 *)&(x))->v)


void FxAtkRingSpawnEcho(BtlUnit *arg0, FxAtkRing *src, s32 arg2) {
    FxAtk *e;
    FxAtkRing *o;
    GphModel *s;
    f32 r;
    GphModel *ss;
    s32 t;

    e = FxAtkAlloc(arg0, 5);
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
    ss = src->model;
    t = src->hitMask;
    s->pos[0] = ss->pos[0];
    UNION_S32(o->hitMask) = t;
    r = ss->pos[2];
    UNION_S16(o->state) = 0;
    s->pos[2] = r;
    o->radius = src->radius;
    o->delay = arg2;
    o->power = src->power;
    o->alpha = 1.0f;
    e->update = FxAtkRingUpdate;
    e->build = FxAtkRingBuild;
    o->t = 0.0f;
}
