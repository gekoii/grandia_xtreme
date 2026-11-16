#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapObstacleGimmicksAdd(AmapWork *a) {
    AmapPlace *ev = a->gimmicks;
    s32 y;
    s32 x;

    for (y = 0; y < a->height; y++) {
        for (x = 0; x < a->width; x++) {
            if (MAP(6, x, y) == 3) {
                ev[a->gimmickCount].type = 0x17;
                ev[a->gimmickCount].x = x;
                ev[a->gimmickCount].y = y;
                a->gimmickCount++;
                MAP(4, x, y) = 0xFF;
            }
        }
    }
}
