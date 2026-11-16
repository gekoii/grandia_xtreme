#include "common.h"
#include "battle/n28.h"

void BtlFadeInit(void) {
    btlFade.flags = 0;
    btlFade.quad.w = 0x280;
    btlFade.quad.h = 0x1C0;
    btlFade.dur = 0;
    btlFade.t = 0;
    btlFade.to.w = 0;
    btlFade.from.w = 0;
    btlFade.quad.originX = 0;
    btlFade.quad.originY = 0;
    btlFade.quad.col[3].w = 0;
    btlFade.quad.col[1].w = 0;
    btlFade.quad.col[2].w = 0;
    btlFade.quad.col[0].w = 0;
}
