/* rodata: 0x006E2740-0x006E2758 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlHudMarkerDraw(void) {
    s32 i;
    BtlHudMarker *fx;

    fx = btlHudMarkers;
    SprBegin(0.0f, 0.0f);
    for (i = 0; i < 0x20; i++, fx++) {
        if (fx->type != 0) {
            switch (fx->type) {
            case 1:
                BtlHudMarkerEvadeDraw(fx);
                break;
            case 2:
                BtlHudMarkerMissDraw(fx);
                break;
            case 4:
                BtlHudMarkerResistDraw(fx);
                break;
            case 5:
                BtlHudMarkerLevelDownDraw(fx);
                break;
            case 6:
                BtlHudMarkerFailDraw(fx);
                break;
            }
        }
    }
    SprFlush();
}
