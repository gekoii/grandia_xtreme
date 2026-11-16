#include "common.h"
#include "field/n13.h"

void MapWallSelectLayer(s32 arg0) {
    if (arg0 == 0) {
        mapWallList = mapWallListMove;
        return;
    }
    mapWallList = mapWallListCam;
}
