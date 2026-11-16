#include "common.h"
#include "field/n21.h"

void AmapStartExitGimmicksAdd(AmapWork *a) {
    AmapPlace *e;

    e = a->gimmicks;
    e[a->gimmickCount].type = 1;
    e[a->gimmickCount].x = a->startX;
    e[a->gimmickCount].y = a->startY;
    a->gimmickCount++;
    e[a->gimmickCount].type = 0x20;
    e[a->gimmickCount].x = a->startX;
    e[a->gimmickCount].y = a->startY;
    a->gimmickCount++;
    e[a->gimmickCount].type = 2;
    e[a->gimmickCount].x = a->exitX;
    e[a->gimmickCount].y = a->exitY;
    a->gimmickCount++;
    e[a->gimmickCount].type = 0;
    e[a->gimmickCount].x = a->exitX;
    e[a->gimmickCount].y = a->exitY;
    a->gimmickCount++;
}
