#include "common.h"
#include "field/n15.h"

void MapCacheClearSmall(void) {
    mapLoadState.smallSize = 0xFFFFFFFF;
    mapLoadState.smallBuf = 0xFFFFFFFF;
    mapLoadState.smallId = 0;
}
