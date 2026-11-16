#include "common.h"
#include "battle/n33.h"

void BtlHudMarkerFailUpdate(BtlHudMarker *m) {
    if (m->timer >= 0x40) {
        m->type = 0;
    }
}
