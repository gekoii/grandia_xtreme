#include "common.h"
#include "field/n14.h"

s32 MapPathStoreSmall(void) {
    s32 i;
    s32 slot;
    s32 ret;

    slot = -1;
    if ((u32)(mapPathPointCount - 1) >= 0x40) {
        return -2;
    }
    for (i = 0; i < 8; i++) {
        if (mapPathWork->smallUsed[i] == 0) {
            slot = i;
            break;
        }
    }
    ret = -1;
    if (slot != -1) {
        mapPathWork->smallUsed[slot] = mapPathPointCount;
        for (i = 0; i < mapPathPointCount; i++) {
            mapPathWork->small[slot].x[i] = mapPathWork->px[i];
            mapPathWork->small[slot].y[i] = mapPathWork->py[i];
            mapPathWork->small[slot].z[i] = mapPathWork->pz[i];
        }
        ret = slot;
    }
    return ret;
}
