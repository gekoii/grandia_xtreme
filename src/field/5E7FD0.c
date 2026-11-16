#include "common.h"
#include "field/n17.h"

s32 MapModelSetVisible(s32 arg0, s32 arg1) {
    mapModelTable[arg0].visible = arg1;
    return arg1;
}
