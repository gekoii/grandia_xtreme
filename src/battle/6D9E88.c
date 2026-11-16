#include "common.h"
#include "battle/n33.h"

void BtlHitAddMvpPoints(BtlHit *a, s32 n) {
    if (a->effect != 0) {
        u32 i = a->src.unit;
        if (i < 5) {
            btlUnits[i].mvp += n;
        }
    }
}
