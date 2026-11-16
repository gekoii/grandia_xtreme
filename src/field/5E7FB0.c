#include "common.h"
#include "field/n17.h"

u8 MapModelIsVisible(s32 arg0) {
    return mapModelTable[arg0].visible;
}
