/* rodata: 0x00674518-0x00674528 */
#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapAlcovesPlace(AmapWork *a) {
    u8 t1[4] = {2, 1, 0, 3};
    u8 t2[4] = {2, 3, 0, 1};
    AmapPlace *ev = a->gimmicks;
    AmapItemPlace *ev2 = a->items;
    AmapPlace *ev3 = a->monsters;
    s32 i;
    s32 n;
    s32 dx;
    s32 dy;
    s32 x;
    s32 y;
    s32 d;
    s32 k;

    n = 0;
    dy = 0;
    dx = 0;
    i = 0;
    while (i < 0x80 && n < a->alcoveCount) {
        x = VuRandInt(a->width - 6) + 3;
        y = VuRandInt(a->height - 6) + 3;
        if (MAP(1, x, y) != 1) {
            d = AmapAlcoveSpotFind(x, y);
            if (d != -1) {
                AmapAlcoveCarve(x, y, d);
                k = -1;
                switch (d) {
                case 0:
                    k = 0;
                    dx = 0;
                    dy = 1;
                    break;
                case 1:
                    k = 1;
                    dx = -1;
                    dy = 0;
                    break;
                case 2:
                    k = 2;
                    dx = 0;
                    dy = -1;
                    break;
                case 3:
                    k = 3;
                    dx = 1;
                    dy = 0;
                    break;
                }
                if (k != -1) {
                    n++;
                    MAP(0, x, y) = 1;
                    ev[a->gimmickCount].type = (t2[k] << 6) + 3;
                    ev[a->gimmickCount].x = x;
                    ev[a->gimmickCount].y = y;
                    a->gimmickCount++;
                    d = VuRandInt(100) + 1;
                    if (d < 30 && a->itemCount < a->itemMax) {
                        ev2[a->itemCount].type = 1;
                        ev2[a->itemCount].x = x + dx * 2;
                        ev2[a->itemCount].y = y + dy * 2;
                        ev2[a->itemCount].facing = t1[k] << 6;
                        a->itemCount++;
                        d = 0xFF;
                    }
                    if (d < 60 && a->monsterCount < a->monsterMax) {
                        ev3[a->monsterCount].type = k << 6;
                        ev3[a->monsterCount].x = x + dx;
                        ev3[a->monsterCount].flag = 0;
                        ev3[a->monsterCount].y = y + dy;
                        ev3[a->monsterCount].arg = 0;
                        a->monsterCount++;
                    }
                }
            }
        }
        i++;
    }
}
