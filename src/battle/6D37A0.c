/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void BtlHudMarkerLevelDownUpdate(BtlHudMarker *fx) {
    switch (fx->state) {
    case 0:
        fx->state++;
        fx->unk10 = 120.0f;
        fx->unk14 = 0.0f;
    case 1:
        fx->unk10 += fx->unk14;
        if (fx->unk10 <= 0.0f) {
            fx->unk10 = -fx->unk10;
            fx->unk14 = -fx->unk14 * 0.25f;
            if (ABS(fx->unk14) < 1.0f) {
                fx->unk10 = 0.0f;
                fx->timer = 0;
                fx->state++;
            }
        } else {
            fx->unk14 -= 0.2f;
        }
        break;
    case 2:
        if (fx->timer >= 0x50) {
            fx->type = 0;
        }
        break;
    }
}
