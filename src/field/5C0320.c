#include "common.h"
#include "field/n13.h"

s32 MapWallGet(s32 arg0) {
    if (arg0 < 0) {
        return 0;
    }
    if (arg0 >= mapWallList->count) {
        return 0;
    }
    return (s32)mapWallList->items[arg0];
}
