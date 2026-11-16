#include "common.h"
#include "field/n25.h"

void MiniSpriteSetCell(MiniSprite *o, s16 x, s16 y) {
    o->startX = o->x;
    o->startY = o->y;
    o->goalX = x * o->cellW;
    o->goalY = y * o->cellH;
    o->cellX = x;
    o->cellY = y;
    o->moveTime = 1;
    o->moveCount = 0;
    o->moveFlags = 1;
}
