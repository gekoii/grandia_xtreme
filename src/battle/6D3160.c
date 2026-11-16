#include "common.h"
#include "battle/n33.h"

void BtlHudMarkerEvadeUpdate(BtlHudMarker *fx) {
    switch (fx->state) {
    case 0:
        if (fx->timer >= 0x10) {
            fx->timer = 0;
            fx->state++;
        }
        break;
    case 1:
        if (fx->timer >= 0x40) {
            fx->type = 0;
        }
        break;
    }
}
