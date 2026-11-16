#include "common.h"
#include "field/n25.h"

s32 MiniSpriteMoveTo(MiniSprite *o, s16 x, s16 y, u16 t) {
    if (o->goalX == x && o->goalY == y) {
        return 0;
    }
    o->moveFlags = 1;
    o->startX = o->x;
    o->startY = o->y;
    o->goalX = x;
    o->goalY = y;
    o->moveTime = t;
    o->moveCount = 0;
    return 1;
}
