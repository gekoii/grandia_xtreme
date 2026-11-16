/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n20.h"

s32 MapMiniFloorSelect(f32 x) {
    u32 i;
    s32 sel;

    sel = 0;
    if (mapMiniWork->hdr->floorCount == 1) {
        return 0;
    }
    for (i = 0; i < mapMiniWork->hdr->floorCount; i++) {
        if (mapMiniWork->floorMinY[i] < x) {
            sel = i;
        }
    }
    mapMiniWork->floor = sel;
    return sel;
}
