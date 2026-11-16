/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "battle/unit.h"

s32 BtlUnitMaskImmuneStatus(BtlUnit *arg0, s32 mask) {
    s32 i;
    s32 bit;
    f32 *p;

    bit = 0x10;
    p = arg0->params->regSta;
    for (i = 0; i < 12; i++) {
        if (*p <= 0.0f) {
            mask &= ~bit;
        }
        p++;
        bit <<= 1;
    }
    return mask;
}
