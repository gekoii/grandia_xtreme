#include "common.h"
#include "battle/n33.h"

s32 BtlHudGaugeGetHits(s32 arg0) {
    return btlHudUnitGauges[arg0].hits;
}
