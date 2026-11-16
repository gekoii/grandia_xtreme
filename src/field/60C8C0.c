#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapSoundSpotsScatter(AmapWork *a) {
    AmapPlace *ev = a->gimmicks;
    s32 n = 0;
    s32 i;
    s32 x;
    s32 y;

    for (i = 0; i < 0x100 && n < 5; i++) {
        x = VuRandInt(a->width - 2) + 1;
        y = VuRandInt(a->height - 2) + 1;
        if (MAP(0, x, y) != 1 && MAP(4, x, y) == 0) {
            n++;
            ev[a->gimmickCount].type = 0x1C;
            ev[a->gimmickCount].x = x;
            ev[a->gimmickCount].y = y;
            a->gimmickCount++;
            MAP(4, x, y) = 1;
        }
    }
}
