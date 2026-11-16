#include "common.h"
#include "battle/n33.h"

void BtlHudMarkerMissUpdate(BtlHudMarker *fx) {
    s16 t;

    switch (fx->state) {
    case 0:
        t = fx->timer;
        if (t == 2) {
            if (fx->unit->flags & 1) {
                fx->state = -1;
            }
        } else if (t >= 0x18) {
            fx->timer = 0;
            fx->state++;
        }
        break;
    case 1:
        if (fx->timer >= 0x14) {
            fx->timer = 0;
            fx->state++;
        }
        break;
    case 2:
        if (fx->timer >= 0x40) {
            fx->type = 0;
        }
        break;
    }
}
