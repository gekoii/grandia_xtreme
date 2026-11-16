#include "common.h"
#include "party.h"
#include "battle/n33.h"
#include "protos/battle.h"

#define STATUS(k, bits)                                                                           \
    if (info->status & bits) {                                                                     \
        if ((u->powUp & bits) == bits) {                                                          \
            u->statMod[k] = 0;                                                                         \
            u->powUp &= ~bits;                                                                    \
        } else {                                                                                   \
            u->statMod[k] = u->statMod[k] + info->power * ((rand() >> 15) * (1.0f / 65536.0f) * 0.2f + 0.9f) \
                * ((u->powUp & (1 << k)) ? st->kregPowD[k] : st->kregPowU[k]);                            \
        }                                                                                          \
    }

void BtlHitApplyStatChange(BtlUnit *u, BtlHitResult *info) {
    PtyUnit *st = u->params;

    u->powUp |= info->status;
    STATUS(0, 0x101)
    STATUS(1, 0x202)
    STATUS(2, 0x404)
    STATUS(3, 0x808)
    STATUS(4, 0x1010)
}
