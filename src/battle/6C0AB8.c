/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlHudSetSpColor(s32 cur, s32 max) {
    f32 m;
    f32 c;

    if (cur >= max) {
        if (!(btlTickCount & 4)) {
            SprSetColor(0x40, 0x7F, 0x58);
            return;
        }
        SprSetColor(0x50, 0x78, 0x73);
        return;
    }
    m = max;
    c = cur;
    if (c <= m * 0.25f) {
        SprSetColor(0x70, 0x18, 0);
        return;
    }
    if (c <= m * 0.5f) {
        SprSetColor(0x7F, 0x74, 0x18);
        return;
    }
    SprSetColor(0x7F, 0x7F, 0x7F);
}
