/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphAnimStep(GphAnimState *arg0, f32 dt) {
    GphAnimKey *fr;
    s32 lo;
    s32 loops;
    f32 wait;
    s32 hi;

    if (gphCurAnimSet == NULL) {
        return;
    }
    if (dt == 0.0f) {
        return;
    }
    arg0->time += dt;
    /* goto loop entered at top (the key lookup), with gotos back to it */
    goto top;
    while (1) {
        arg0->key++;
        if (gphCurAnimSet->flags & 2) {
            if (arg0->key >= gphCurAnimSet->keyCount) {
                arg0->key = gphCurAnimSet->keyCount - 1;
            }
        } else if (arg0->key >= gphCurAnimSet->keyCount) {
            arg0->key = 0;
        }
    top:
        fr = GphAnimGetKey(arg0->key);
        if (fr == NULL) {
            lo = 0;
            wait = 1.0f;
            loops = 1;
            hi = gphCurAnimSet->cellCount;
        } else {
            lo = fr->cellStart;
            wait = fr->wait;
            hi = fr->cellEnd + 1;
            loops = fr->loops - 1;
            if (arg0->cell < lo) {
                arg0->cell = lo;
            }
        }
        if (gphCurAnimSet->cellCount >= 2) {
            if (gphCurAnimSet->flags & 8) {
                if (arg0->rep >= loops) {
                    arg0->rep = 0;
                    arg0->cell = lo + VuRandInt(hi - lo);
                } else {
                    arg0->rep++;
                }
            } else {
                if (arg0->rep >= loops) {
                    arg0->rep = 0;
                    arg0->cell++;
                    if (gphCurAnimSet->flags & 2) {
                        if (arg0->cell >= hi) {
                            arg0->cell = hi - 1;
                            arg0->done = 1;
                        }
                    } else if (arg0->cell >= hi) {
                        arg0->cell = lo;
                    }
                } else {
                    arg0->rep++;
                }
            }
        }
        if (!(wait <= arg0->time)) {
            return;
        }
        arg0->time -= wait;
        if (gphCurAnimSet->keyCount == 0) {
            goto top;
        }
        if (gphCurAnimSet->flags & 8) {
            arg0->key = VuRandInt(gphCurAnimSet->keyCount);
            goto top;
        }
    }
}
