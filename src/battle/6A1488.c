/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void FxStatusIconMove(FxStatusIcon *arg0, f32 target) {
    f32 d;
    f32 cur;

    if (arg0->state > 0) {
        if (arg0->alpha < 1.0f) {
            arg0->alpha += 0.015625f;
            if (arg0->alpha > 1.0f) {
                arg0->alpha = 1.0f;
            }
        }
        cur = arg0->pos;
        d = cur - target;
        if (ABS(d) < 0.5f) {
            arg0->pos = target;
            return;
        }
        d *= 0.125f;
        if (ABS(d) < 0.5f) {
            d = d > 0.0f ? 0.5f : -0.5f;
        }
        arg0->pos = cur - d;
    } else if (arg0->state < 0) {
        if (arg0->alpha > 0.0f) {
            arg0->alpha -= 0.03125f;
            if (arg0->alpha <= 0.0f) {
                arg0->alpha = 0.0f;
                arg0->state = 0;
            }
        }
        arg0->scale += 0.03125f;
    }
}
