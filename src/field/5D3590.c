#include "common.h"
#include "protos/field.h"
#include "field/n15.h"


void MapRippleUpdate(void) {
    MapRippleSurf *s;
    u32 i;

    if (mapRippleWork.count != 0) {
        for (i = 0; i < mapRippleWork.count; i++) {
            s = mapRippleWork.surf[i];
            if (mapRippleWork.kind[i] == 1) {
                if (mapRippleWork.timer[i] == 0) {
                    GphRippleAddRect(s, VuRandInt(s->w), VuRandInt(s->h), s->w >> 4, s->h >> 4, VuRandInt(0x80));
                    mapRippleWork.timer[i] = VuRandInt(0x3C);
                } else {
                    mapRippleWork.timer[i]--;
                }
                GphRippleUpdate(s, 0xB);
            }
        }
    }
}
