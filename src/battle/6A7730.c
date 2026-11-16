/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void IpPanelStatusIconMove(FxStatusIcon *arg0, f32 arg1) {
    f32 d;

    if (arg0->state > 0) {
        if (arg0->alpha < 1.0f) {
            arg0->alpha += 0.03125f;
            if (arg0->alpha > 1.0f) {
                arg0->alpha = 1.0f;
            }
        }
        d = arg0->pos - arg1;
        if (ABS(d) < 0.5f) {
            arg0->pos = arg1;
            return;
        }
        d *= 0.125f;
        if (ABS(d) < 0.5f) {
            d = (d > 0.0f) ? 0.5f : -0.5f;
        }
        arg0->pos -= d;
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
