/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"


#define RND() ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f)

void FxAtkMultiHitUpdate(FxAtk *e) {
    FxAtkMultiHit *o = e->work;
    BtlUnit *c = o->target;
    GphModel *t;
    VuVec3 v;
    s32 ang;
    s32 i;

    switch (o->state) {
    case 0:
        if (c->params->status & 0x8000) {
            o->state++;
            return;
        }
        if (o->timer <= 0) {
            BtlChrGetPartPos(&v, c, 1);
            o->alpha[o->next] = 1.0f;
            o->models[o->next]->pos[0] = v.x + RND() * 30.0f;
            o->models[o->next]->pos[1] = v.y + RND() * 30.0f;
            o->models[o->next]->pos[2] = v.z + RND() * 30.0f;
            o->models[o->next]->rot[0] = rand() >> 15;
            o->models[o->next]->rot[1] = rand() >> 15;
            o->models[o->next]->rot[2] = rand() >> 15;
            o->models[o->next]->scale[0] = o->models[o->next]->scale[1] = o->models[o->next]->scale[2] = 0.0f;
            ang = BtlAtan2Angle(e->unit->pos[0] - c->pos[0], e->unit->pos[2] - c->pos[2]);
            FxAtkHitRegister(e->unit, c, ang + RND() * 4096.0f, 0, o->power);
            o->next++;
            if (o->next >= o->count) {
                o->state++;
            } else {
                o->timer = 8 - o->next * 2;
            }
        }
        o->timer--;
        break;
    case 1:
        for (i = 0; i < 3; i++) {
            if (o->alpha[i] > 0.0f) {
                break;
            }
        }
        if (i >= 3) {
            e->frameStamp = btlFrameCount;
            o->state++;
        }
        break;
    case 2:
        if (e->frameStamp != btlFrameCount) {
            FxAtkFree(e);
            return;
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        if (o->alpha[i] > 0.0f) {
            t = o->models[i];
            t->scale[0] = t->scale[1] = t->scale[2] = t->scale[0] + 0.05f;
            if (t->scale[0] > 0.5f) {
                o->alpha[i] = (1.0f - t->scale[0]) * 2;
            }
        }
    }
}
