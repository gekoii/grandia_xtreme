/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"

s32 AiTargetCanBeCancelled(BtlUnit *arg0) {
    BtlUnit *e;
    f32 f;

    e = &btlUnits[arg0->target];
    f = e->ipPos + (e->ipMode != 2 ? 1.0f : 0.0f);
    if (f < 1.25f && f > 0.1f) {
        return 1;
    }
    return 0;
}
