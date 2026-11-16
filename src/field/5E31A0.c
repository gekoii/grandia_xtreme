#include "common.h"
#include "field/n17.h"
#include "protos/field.h"

void MapCollInit(void) {
    MapBorderInit();
    MapPolyInit();
    mapCollFlags = 0xFF;
}

s32 MapCollGetFlags(void) {
    return mapCollFlags;
}

s32 MapCollGetObjFlags(void) {
    return mapCollFlags;
}

void MapCollSetFlags(s32 arg0) {
    mapCollFlags = arg0;
}
