#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"

void VuVecCopy(void *, void *);

s32 BtlEnemyActivate(s32 id, s32 start) {
    s32 i;
    BtlUnit *u;
    PtyUnit *s;

    u = &btlUnits[start];
    i = start;
    for (; i < 16; i++, u++) {
        if (u->state != 0) {
            continue;
        }
        if (u->def == NULL) {
            continue;
        }
        s = u->params;
        if (s->id == id) {
            break;
        }
    }
    if (!(i < 16)) {
        return -1;
    }
    btlEnemyAliveCount++;
    s->status = BtlUnitMaskImmuneStatus(u, s->status2);
    s->hp = s->maxHp;
    u->retState = 0x80;
    u->powUp = 0;
    u->flags = 0;
    u->retPhase = 0;
    BtlUnitSetState(u, 0x80);
    u->phase = 1;
    u->subPhase = 0;
    BtlChrSetMotion(u, 0);
    u->frameSpeed = 1.0f;
    IpIconSetPos(u->id, 1.5f);
    u->ipMode = 1;
    u->flags &= 0xFFFFFF7F;
    u->action = 0x7F;
    u->ipRet = 1.0f;
    u->ipDamage = 0;
    u->ipRate = 1.0f;
    u->ipStop = 0;
    u->ipPos = 1.0f;
    u->ipSpeed = 0;
    u->scopeType = 0;
    u->ai = 0;
    u->turn = 0;
    u->flag = 0;
    BtlUnitSetFreeSpotTarget(u);
    VuVecCopy(u->pos, u->moveTarget);
    BtlUnitSelectTarget(u, 1, 0x1003);
    u->params->angryProb = 0;
    u->item = 0;
    u->treasure = 0;
    if (u->model != NULL) {
        u->ambient[0] = u->ambient[1] = u->ambient[2] = 0.0f;
        ChrModelRestorePartAttr(u->model, u->def->modelPack);
    }
    u->alpha = 0;
    u->scale[2] = 1.0f;
    u->scale[1] = 1.0f;
    u->scale[0] = 1.0f;
    return u->id;
}
