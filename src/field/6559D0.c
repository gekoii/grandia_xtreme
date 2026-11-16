/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

extern void VuVecCopy(void *, void *);

void WmapRippleUpdate(void) {
    WmapRipple *e;
    s32 i;
    u16 max;
    s32 cur;

    max = wmapRipples->period;
    cur = wmapRipples->timer;
    if ((wmapRipples->flags & 1) && cur == 0) {
        for (i = 0; i < wmapRipples->count; i++) {
            e = &wmapRipples->rings[i];
            if (e->active == 0) {
                e->active = 1;
                VuVecCopy(e->pos, wmapCursorPos);
                e->t = 0.0f;
                e->end = 5760.0f;
                break;
            }
        }
    }
    for (i = 0; i < wmapRipples->count; i++) {
        e = &wmapRipples->rings[i];
        if (e->active != 0) {
            e->t += 32.0f;
            if (e->end <= e->t) {
                e->active = 0;
            }
        }
    }
    cur++;
    if (max < cur) {
        cur = 0;
    }
    wmapRipples->timer = cur;
}
