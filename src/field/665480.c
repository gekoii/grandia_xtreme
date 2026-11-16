#include "common.h"
#include "field/n27.h"

void EvtMsgTileRectClear(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x;
    s32 y;
    s32 row;

    for (y = y0; y <= y1; y++) {
        row = y * EVT_MSG_TILE_COLS;
        for (x = x0; x <= x1; x++) {
            evtMsgTileGrid[row + x] = 0;
        }
    }
}
