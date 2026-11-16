#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"
#include "sys.h"

/* MgcGetHitData record */
typedef struct {
    u8 pad0[0x18];
    u8 unk18;
} Info;

/* view of the enemy model binding (btlHandoff.enemyModels) */
typedef struct {
    u8 pad0[0x188];
    s32 unk188;
} PrmA;

typedef struct {
    PrmA *unk0;
    s32 unk4;
} Prm;



void FxDelayedCastStart(BtlUnit *c, f32 x, f32 z) {
    FxAtk *e;
    FxDelayedCast *o;
    Prm *p;
    Prm *p2;
    GphModel *m;
    GphModel *m2;
    s32 old;
    s32 t;

    e = FxAtkAlloc(c, 2);
    if (e == NULL) {
        return;
    }
    old = MemHeapGetCurrentId();
    MemHeapSelect(sysBattleHeap);
    o = MemAlloc16C(0x220);
    e->work = o;
    if (o == NULL) {
        FxAtkFree(e);
        MemHeapSelect(old);
        return;
    }
    o->state = 0;
    e->update = FxDelayedCastUpdate;
    e->build = FxDelayedCastBuild;
    o->hitData = MgcGetHitData();
    switch (((Info *)o->hitData)->unk18) {
    case 0x33:
        o->timer = 300;
        break;
    case 0x34:
        o->timer = 450;
        break;
    case 0x35:
        o->timer = 600;
        break;
    case 0x36:
        o->timer = 750;
        break;
    default:
        o->timer = 900;
        break;
    }
    o->unit = *c;
    o->unit.pos[0] = x;
    o->unit.pos[2] = z;
    o->unit.unk1D8 = 0;
    o->pulse = 0;
    if (c->params->id == 0xD1) {
        p = btlHandoff.enemyModels[c->def->slot - 4];
        t = FilePacGetModel(p->unk4, 2);
        o->unit.pos[1] = 0.0f;
        m = BtlModelCreateWithPool(t, &e->pool);
        o->model = m;
        if (m == NULL) {
            FxAtkFree(e);
            MemHeapSelect(old);
            return;
        }
        m->motion[0].motion = (GphMotion *)p->unk0->unk188;
        ChrModelClearDeformBufs(m);
        fxDelayedCastHide = 0;
    } else {
        o->unit.pos[1] = 150.0f;
        p2 = SysCommonModelGet(0x3D);
        m2 = BtlModelCreateWithPool((s32)p2, &e->pool);
        o->model = m2;
        if (m2 == NULL) {
            FxAtkFree(e);
            MemHeapSelect(old);
            return;
        }
        t = MemAlloc32(GphDrawListSize(p2->unk4));
        e->drawList = t;
        if (t == 0) {
            FxAtkFree(e);
            MemHeapSelect(old);
            return;
        }
        GphDrawListInit(p2->unk4, t);
        GphDrawListSet(e->drawList);
        GphDrawListClear(0);
        e->shadowCb = EnvShadowAddCallback(FxDelayedCastShadowBuild, FxDelayedCastShadowDraw, e);
        e->flags |= 0x100;
    }
    e->flags |= 0x200;
    MemHeapSelect(old);
}
