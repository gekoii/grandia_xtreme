#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void GphRippleApply8(MapRippleSurf *);

void MapRippleApply(void) {
    u32 i;

    if (mapRippleWork.count != 0) {
        for (i = 0; i < mapRippleWork.count; i++) {
            if (GphDrawListHasTex(mapRippleWork.slot[i]) != 0) {
                GphRippleApply8(mapRippleWork.surf[i]);
            }
        }
    }
}
