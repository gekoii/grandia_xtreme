/* rodata: 0x006E2720-0x006E2738 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlHudMarkerUpdate(void) {
    s32 i;
    BtlHudMarker *fx;

    fx = btlHudMarkers;
    for (i = 0; i < 0x20; i++, fx++) {
        if (fx->type != 0) {
            switch (fx->type) {
            case 1:
                BtlHudMarkerEvadeUpdate(fx);
                break;
            case 2:
                BtlHudMarkerMissUpdate(fx);
                break;
            case 4:
                BtlHudMarkerResistUpdate(fx);
                break;
            case 5:
                BtlHudMarkerLevelDownUpdate(fx);
                break;
            case 6:
                BtlHudMarkerFailUpdate(fx);
                break;
            }
            fx->timer++;
        }
    }
}
