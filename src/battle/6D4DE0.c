#include "common.h"
#include "party.h"
#include "battle/n33.h"
#include "protos/battle.h"

void VuVecCopy(void *, void *);

void BtlHitQueueAdd(BtlHitSource *r, BtlUnit *t, s32 arg2, PtyMoveEffect *info) {
    BtlHit *h;
    f32 *src;
    s32 n;

    if (t->state <= 0 || (t->params->bitCsf & 8)) {
        BtlHitNextClear();
        return;
    }
    if (btlHitQueueCount >= 32) {
        BtlHitNextClear();
        return;
    }
    if (btlHitNextVictimHandler == NULL && t->state != 0x7F && (t->params->status & 2) && (info->hitFlags & 4)) {
        BtlHitNextClear();
        return;
    }
    n = btlHitQueueCount;
    h = &btlHitQueue[n];
    src = btlHitNextSrcPos;
    h->target = t;
    h->angle = arg2;
    h->flags = btlHitNextFlags;
    btlHitQueueCount = n + 1;
    h->victimHandler = btlHitNextVictimHandler;
    btlHitNextFlags = 0;
    h->effect = info;
    btlHitNextVictimHandler = NULL;
    if (src[3] > 0.0f) {
        VuVecCopy(h->srcPos, src);
        h->flags |= BTL_HIT_SRC_POS;
        src[3] = 0.0f;
    }
    h->callback = btlHitNextCallback;
    btlHitNextCallback = NULL;
    h->src.unit = r->unit;
    h->src.action = r->action;
    h->src.skillId = r->skillId;
    h->callbackArg = btlHitNextCallbackArg;
    h->src.body = r->body;
    h->src.params = r->params;
    h->src.statusProb = r->statusProb;
    h->src.status = r->status;
    if (r->unit < 4) {
        if (r->action < 2) {
            h->flags |= BTL_HIT_PARTY_ATTACK;
        } else if (r->action == 3 && ptyMoveTable[r->skillId].target == 1 && h->effect->kind == 5) {
            h->flags |= BTL_HIT_COMBO_FINISH;
        }
    }
    if (info->hitFlags & 2) {
        h->flags |= BTL_HIT_DATA_FLAG2;
    }
}
