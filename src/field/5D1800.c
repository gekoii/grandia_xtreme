#include "common.h"
#include "field/n15.h"

void MapLoadStateIdle(void) {
    mapLoadState.state = MAP_LOAD_IDLE;
    mapLoadState.sub = -1;
}

s32 MapLoadGetState(void) {
    return mapLoadState.state;
}
