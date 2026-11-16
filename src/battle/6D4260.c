#include "common.h"
#include "battle/n33.h"

void BtlHudMarkerSpawn(BtlUnit *u, s8 type) {
    BtlHudMarker *fx;
    s32 i;
    s32 j;

    fx = &btlHudMarkers[btlHudMarkerNext];
    if (type == 3) {
        j = btlHudMarkerNext;
        for (i = 0; i < 0x20; i++) {
            j--;
            if (j <= 0) {
                j = 0x1F;
            }
            fx = &btlHudMarkers[j];
            if (fx->type == 2 && fx->unit == u) {
                fx->type = 0;
                return;
            }
        }
    } else {
        fx->type = type;
        fx->state = 0;
        fx->timer = 0;
        fx->x = u->pos[0];
        fx->unit = u;
        fx->z = u->pos[2];
        fx->unk10 = fx->unk14 = 0.0f;
        btlHudMarkerNext++;
        if (btlHudMarkerNext >= 0x20) {
            btlHudMarkerNext = 0;
        }
    }
}
