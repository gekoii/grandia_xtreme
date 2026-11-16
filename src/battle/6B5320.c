/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"

void EnvDimUpdate(void) {
    EnvDim *r = &envDim;

    if (r->step != 0.0f) {
        envDim.cur += r->step;
        if (r->step > 0.0f ? r->target <= envDim.cur : envDim.cur <= r->target) {
            r->cur = r->target;
            r->step = 0.0f;
        }
    }
}
