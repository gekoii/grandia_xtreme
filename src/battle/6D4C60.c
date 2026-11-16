#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"


void BtlHitQueueAddAttack(BtlUnit *c, BtlUnit *t, s32 arg2, s32 arg3) {
    BtlHitSource r;
    BtlUnit *o;

    if (c->partsChr >= 0) {
        o = &btlUnits[c->partsChr];
    } else {
        o = c;
    }
    if (t->state <= 0 || (t->params->bitCsf & 8)) {
        BtlHitNextClear();
        return;
    }
    r.unit = o->id;
    r.action = o->action;
    r.skillId = o->skillId;
    r.body = o->body;
    r.statusProb = 0.0f;
    r.status = 0;
    r.params = o->params;
    if (arg3 == 0) {
        if (o->id < 4) {
            if (o->action < 2) {
                arg3 = (s32)ptyMoveTable[o->skillId].levels[o->skillLevel - 1].effects;
                if (o->combo <= 0) {
                    r.status = o->params->eqpBitSta;
                    r.statusProb = o->params->eqpStaProb;
                    if (o->action == 0) {
                        arg3 += 0x1C;
                    }
                }
            } else {
                arg3 = (s32)MgcGetHitData();
            }
        } else if (o->skillId == 0) {
            arg3 = (s32)BTL_UNIT_ACT(o)->level->effects;
        } else {
            arg3 = ((s32 (*)(void))MgcGetHitData)(); /* through an s32 cast: a plain call changes the block layout */
        }
    }
    BtlHitQueueAdd(&r, t, arg2, arg3);
}
