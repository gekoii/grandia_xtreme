#include "common.h"
#include "field/n15.h"

void MapReenterRequest(s32 arg0) {
    if (mapLoadState.state >= 2 || mapLoadState.state == MAP_LOAD_IDLE) {
        mapLoadState.state = MAP_LOAD_READ;
        mapNo.entrance = arg0;
        mapNo.prev = mapNo.cur;
    }
}
