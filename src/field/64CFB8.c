#include "common.h"
#include "field/n25.h"

void MiniSpriteSetPos(MiniSprite *o, s16 x, s16 y) {
    o->x = x;
    o->y = y;
    o->startX = x;
    o->goalX = x;
    o->startY = y;
    o->goalY = y;
    o->cellX = 0;
    o->cellY = 0;
    o->moveTime = 0;
    o->moveCount = 0;
    o->moveFlags = 0;
}
