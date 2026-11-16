#include "common.h"
#include "field/n25.h"

void WmapRippleInit(void) {
    s32 i;

    wmapRipples->flags = 0;
    wmapRipples->count = 10;
    for (i = 9; i >= 0; i--) {
        wmapRipples->rings[i].active = 0;
    }
    wmapRipples->period = 20;
    wmapRipples->timer = 0;
}
