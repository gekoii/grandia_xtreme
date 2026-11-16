#include "common.h"
#include "protos/field.h"


void EvtMsgTileLine(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 i;

    if (x0 == x1) {
        for (i = y0; i <= y1; i++) {
            EvtMsgTilePut(x0, i, 0x11);
        }
    } else {
        for (i = x0; i <= x1; i++) {
            EvtMsgTilePut(i, y0, 0x12);
        }
    }
}
