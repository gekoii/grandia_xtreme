#include "common.h"
#include "battle/n28.h"

void BtlFadeStart(s8 flags, s32 to, s32 dur) {
    s32 from;

    btlFade.flags = flags;
    from = btlFade.to.w;
    btlFade.to.w = to;
    btlFade.dur = dur;
    btlFade.from.w = from;
    btlFade.t = 0;
}
