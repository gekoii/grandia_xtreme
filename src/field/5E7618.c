#include "common.h"
#include "field/n17.h"

s32 MapCollModelGet(s32 arg0) {
    if (arg0 == 0) {
        return (s32)mapCollModel;
    }
    return (s32)mapCamCollModel;
}

void MapCollModelSet(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        mapCollModel = (struct GphModel *)arg0;
        return;
    }
    mapCamCollModel = (struct GphModel *)arg0;
}
