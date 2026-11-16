#include "common.h"
#include "field/n21.h"

s32 AmapGenGetBufSize(AmapWork *arg0) {
    if (arg0->width < 0x10U) {
        arg0->width = 0x10U;
    }
    if (arg0->height < 0x10U) {
        arg0->height = 0x10U;
    }
    arg0->bufSize = 0xC4430;
    return 0xC4430;
}

s32 AmapGenGetCellBufSize(AmapWork *arg0) {
    arg0->cellBufSize = 0x7000;
    return 0x7000;
}
