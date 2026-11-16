#include "common.h"
#include "field/n22.h"

s32 AmapGetCellType(AmapWork *arg0, s32 arg1, s32 arg2) {
    if (arg1 < 0 || arg2 < 0 || arg0 == NULL || arg0->tiles == NULL || arg0->width == 0 || arg0->height == 0 ||
        arg2 >= arg0->width || arg1 >= arg0->height) {
        return -1;
    }
    return arg0->tiles[arg0->width * arg1 + arg2] & 0x3F;
}
