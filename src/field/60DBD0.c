#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapMonstersScatter(AmapWork *a) {
    AmapPlace *ev;
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    s32 v;

    if (a->mapType != 2) {
        n = a->monsterCount;
        ev = a->monsters;
        for (i = 0; i < 0x100 && n < a->monsterMax; i++) {
            x = VuRandInt(a->width - 2) + 1;
            y = VuRandInt(a->height - 2) + 1;
            if (MAP(0, x, y) == 1 && MAP(1, x, y) != 1 && MAP(4, x, y) == 0) {
                MAP(4, x, y) = 1;
                ev[a->monsterCount].type = 0;
                ev[a->monsterCount].x = x;
                ev[a->monsterCount].flag = 0;
                ev[a->monsterCount].y = y;
                ev[a->monsterCount].arg = 0;
                AmapFloorMask8(x, y, &v);
                ev[a->monsterCount].type &= 0x3F;
                if (v & 4) {
                    ev[a->monsterCount].type |= 0x40;
                } else if (v & 0x10) {
                    ev[a->monsterCount].type |= 0x80;
                } else if (v & 0x40) {
                    ev[a->monsterCount].type |= 0xC0;
                }
                n++;
                a->monsterCount++;
            }
        }
    }
}
