/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"

f32 BtlUnitGetMoveSpeed(BtlUnit *arg0) {
    BtlUnit *u;
    f32 k;
    u16 f;

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    if (arg0->flags & 0x100) {
        return 0.0f;
    }
    if (btlActingUnit != 0xFF) {
        k = 0.01f;
    } else {
        f = u->powUp;
        if (f & 0x10) {
            k = 0.002f;
        } else if (f & 0x1000) {
            k = 0.02f;
        } else {
            k = 0.01f;
        }
    }
    k = k * ((f32)u->params->params[5] * 9.6f);
    if (arg0->id < 4 && k > 48.0f) {
        k = 48.0f;
    }
    return k;
}
