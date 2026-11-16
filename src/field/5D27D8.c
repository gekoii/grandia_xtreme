#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

s32 MapIsCached(u16 arg0) {
    if (arg0 == mapLoadState.largeId && FldMemGetCur() < mapLoadState.largeBuf) {
        return 1;
    }
    return mapLoadState.smallId == arg0;
}
