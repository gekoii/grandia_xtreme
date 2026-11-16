#include "common.h"
#include "battle/n33.h"

void VuVecCopy(VuVec4 *, VuVec4 *);

/* Three parameters: BtlHitResolve (0x006D5398) passes two through a cast, leaving flags in a2 as is. */
void BtlHudDamageNumSpawn(s32 value, VuVec4 *pos, s32 flags) {
    BtlHudDamageNum *d;
    s32 i;

    d = btlHudDamageNums;
    for (i = 0; i < 0x20; i++, d++) {
        if (d->active == 0) {
            break;
        }
    }
    if (i < 0x20) {
        d->value = value;
        d->active = 1;
        d->flags = flags;
        d->timer = 0;
        VuVecCopy(&d->pos, pos);
        if (flags & 0x10) {
            d->dy = -96.0f;
            d->scale = 2.0f;
        } else {
            d->dy = 0.0f;
            if (flags & 0x20) {
                d->scale = 1.3f;
            } else {
                d->scale = 1.0f;
            }
        }
        d->dx = d->stackY = 0.0f;
    }
}
