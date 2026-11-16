#include "common.h"
#include "battle/n33.h"
#include "party.h"
#include "protos/battle.h"

void VuVecCopy(void *, void *);

void BtlHitQueueAddDamage(BtlUnit *t, s32 a1, f32 f, s32 a2, s32 a3, s8 a4) {
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
    n = btlHitQueueCount;
    h = &btlHitQueue[n];
    src = btlHitNextSrcPos;
    h->target = t;
    h->angle = a2;
    btlHitQueueCount = n + 1;
    h->victimHandler = btlHitNextVictimHandler;
    h->effect = NULL;
    btlHitNextVictimHandler = NULL;
    h->flags = a3;
    if (src[3] > 0.0f) {
        VuVecCopy(h->srcPos, src);
        h->flags |= BTL_HIT_SRC_POS;
        src[3] = 0.0f;
    }
    h->callback = btlHitNextCallback;
    btlHitNextCallback = NULL;
    h->callbackArg = btlHitNextCallbackArg;
    h->status = 0;
    h->damage = a1 > -1 ? a1 : 0;
    h->unk58 = a4;
    h->unk5C = f;
    h->src.status = 0;
    h->src.statusProb = 0.0f;
    h->unk60 = 0;
}
