#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


s32 MiniCursorMove(MiniSprite *o, s16 xmin, s16 ymin, s16 xmax, s16 ymax, u16 time, u8 mode) {
    MiniInput *pad;

    if (o->moveCount != 0) {
        return 0;
    }
    pad = &miniUi->input;
    if (MINI_INPUT_DIR(pad) == 0) {
        return 0;
    }
    if (pad->dy == 0 && o->cellW == 0) {
        return 0;
    }
    if (pad->dx == 0 && o->cellH == 0) {
        return 0;
    }
    o->cellX += pad->dx;
    o->cellY += pad->dy;
    switch (mode) {
    case 0:
        if (xmax < o->cellX) {
            o->cellX = xmin;
        }
        if (o->cellX < xmin) {
            o->cellX = xmax;
        }
        if (ymax < o->cellY) {
            o->cellY = ymin;
        }
        if (o->cellY < ymin) {
            o->cellY = ymax;
        }
        break;
    case 1:
        if (xmax < o->cellX) {
            o->cellX = xmax;
        }
        if (o->cellX < xmin) {
            o->cellX = xmin;
        }
        if (ymax < o->cellY) {
            o->cellY = ymax;
        }
        if (o->cellY < ymin) {
            o->cellY = ymin;
        }
        break;
    }
    if (MiniSpriteMoveTo(o, o->cellX * o->cellW, o->cellY * o->cellH, time) == 0) {
        return 0;
    }
    o->waveT = 0;
    return 1;
}
