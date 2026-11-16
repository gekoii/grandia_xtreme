#include "common.h"
#include "field/n25.h"

void MiniBallReelSetTiming(u8 id, u8 n) {
    MiniBallPrize *s;
    u32 i;

    for (i = 0; i < 6; i++) {
        s = &miniBallWork->game.prizes[i];
        if (s->reel == id) break;
    }
    s->times[0] = miniBallData->reelTimes[n * 3 - 3];
    s->times[1] = miniBallData->reelTimes[n * 3 - 2];
    s->times[2] = miniBallData->reelTimes[n * 3 - 1];
}
