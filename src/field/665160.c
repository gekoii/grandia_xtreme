#include "common.h"
#include "field/n27.h"

void EvtMsgTileFramePlain(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x;
    s32 y;
    s32 t;

    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            if (x == x0) {
                if (y == y0) {
                    t = 1;
                } else {
                    t = 0xD;
                    if (y == y1) {
                        t = 0xB;
                    } else if (y == y0 + 1) {
                        t = 0x1C;
                    }
                }
            } else if (x == x1) {
                if (y == y0) {
                    t = 2;
                } else {
                    t = 0xE;
                    if (y == y1) {
                        t = 0xC;
                    } else if (y == y0 + 1) {
                        t = 0x1D;
                    }
                }
            } else if (y == y0) {
                t = 5;
            } else {
                t = ((y ^ y1) == 0) ? 0xF : 0;
            }
            if (t == 0) t = 0x60;
            evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x] = t;
        }
    }
}
