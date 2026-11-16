#include "common.h"
#include "protos/field.h"


void EvtMsgTileFrame(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x;
    s32 y;
    s32 t;

    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            if (x == x0) {
                if (y == y0) {
                    t = 1;
                } else if (y == y1) {
                    t = 3;
                } else {
                    t = 5;
                }
            } else if (x == x1) {
                if (y == y0) {
                    t = 2;
                } else if (y == y1) {
                    t = 4;
                } else {
                    t = 6;
                }
            } else if (y == y0) {
                t = 7;
            } else {
                t = ((y ^ y1) == 0) ? 8 : 0;
            }
            EvtMsgTilePut(x, y, t);
        }
    }
}
