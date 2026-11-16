/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"

void VuVecCopy(void *, void *);


s32 BtlHitQueueAddStatus(BtlUnit *t, u32 mask, s32 a2, u32 flags, s32 a4) {
    BtlHit *h;
    f32 *src;
    s32 n;
    s32 i;
    u32 bit;
    f32 *p;

    if (t->params->bitCsf & 8) {
        BtlHitNextClear();
        return 0;
    }
    if (!(flags & 0x200)) {
        p = t->params->regSta;
        bit = 0x10;
        for (i = 0; i < 12; i++) {
            if (*p <= 0.0f) {
                if (mask & bit) {
                    flags |= 0x400;
                }
                mask &= ~bit;
            }
            p++;
            bit <<= 1;
        }
    }
    if (mask == 0 && a2 == 0) {
        if (flags & 0x400) {
            BtlHudMarkerSpawn(t, 4);
        }
        BtlHitNextClear();
        return 0;
    }
    if (btlHitQueueCount >= 32) {
        BtlHitNextClear();
        return 0;
    }
    n = btlHitQueueCount;
    h = &btlHitQueue[n];
    src = btlHitNextSrcPos;
    h->effect = NULL;
    h->target = t;
    btlHitQueueCount = n + 1;
    h->victimHandler = btlHitNextVictimHandler;
    /* read through an enum type (outside int's alias set): the only form that matches */
    h->angle = *(enum { ANG0 } *)&t->rot[1];
    btlHitNextVictimHandler = NULL;
    h->flags = flags | 0x8000;
    if (src[3] > 0.0f) {
        VuVecCopy(h->srcPos, src);
        h->flags |= BTL_HIT_SRC_POS;
        src[3] = 0.0f;
    }
    h->callback = btlHitNextCallback;
    btlHitNextCallback = NULL;
    h->callbackArg = btlHitNextCallbackArg;
    h->status = a2;
    h->damage = 0;
    h->unk58 = 0;
    h->unk5C = 0.0f;
    h->src.statusProb = 0.0f;
    h->src.status = mask;
    h->unk60 = a4;
    return 1;
}
