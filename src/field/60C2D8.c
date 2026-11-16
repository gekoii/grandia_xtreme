#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapGimmicksScatter(AmapWork *a, u16 mode, u16 dirmode, u16 lay, u16 base, u16 rng, u16 count) {
    s32 i;
    s32 n;
    AmapPlace *ev = a->gimmicks;
    s32 dir;
    s32 x;
    s32 y;
    s32 flag;
    s32 m;

    dir = 0;
    n = 0;
    for (i = 0; i < 0x100 && n < count && a->gimmickCount < 0x100; i++) {
        x = VuRandInt(a->width - 2) + 1;
        y = VuRandInt(a->height - 2) + 1;
        if (lay == 0) {
            if (MAP(4, x, y) != 0) continue;
        } else if (lay == 2) {
            if (MAP(4, x, y) != 0 || MAP(5, x, y) != 0) continue;
        } else {
            if (MAP(5, x, y) != 0) continue;
        }
        flag = 0;
        switch (mode) {
        case 0:
            if (MAP(0, x, y) == 1) flag = 1;
            if (MAP(0, x, y - 1) == 1) flag = 1;
            if (MAP(0, x + 1, y) == 1) flag = 1;
            if (MAP(0, x, y + 1) == 1) flag = 1;
            if (MAP(0, x - 1, y) == 1) flag = 1;
            if (MAP(1, x, y - 1) == 1) flag = 1;
            if (fldMapCells[x + (fldMapLayerSize + y * fldMapWidth) + 1] == 1) flag = 1;
            if (MAP(1, x, y + 1) == 1) flag = 1;
            if (fldMapCells[x + (fldMapLayerSize + y * fldMapWidth) - 1] == 1) flag = 1;
            break;
        case 1:
            if (MAP(0, x, y) != 2) flag = 1;
            if (MAP(2, x, y) == 1) flag = 1;
            break;
        case 2:
            if (MAP(0, x, y) != 1) flag = 1;
            break;
        case 3:
            if (MAP(0, x, y) == 0) flag = 1;
            break;
        case 5:
            if (MAP(0, x, y) != 2) flag = 1;
            if (MAP(2, x, y) != 0) flag = 1;
            if (MAP(0, x, y - 1) != 2) flag = 1;
            if (MAP(0, x + 1, y) != 2) flag = 1;
            if (MAP(0, x, y + 1) != 2) flag = 1;
            if (MAP(0, x - 1, y) != 2) flag = 1;
            break;
        }
        if (flag) continue;
        if (dirmode == 0) {
            dir = VuRandInt(4);
        } else {
            m = 0;
            if (MAP(0, x, y - 1) == 0) m |= 1;
            if (MAP(0, x + 1, y) == 0) m |= 2;
            if (MAP(0, x, y + 1) == 0) m |= 4;
            if (MAP(0, x - 1, y) == 0) m |= 8;
            if (m == 0) {
                flag = 1;
            } else {
                if (m & 1) dir = 0;
                if (m & 2) dir = 1;
                if (m & 4) dir = 2;
                if (m & 8) dir = 3;
            }
        }
        if (flag) continue;
        if (lay == 0) {
            MAP(4, x, y) = 1;
        } else if (lay == 2) {
            MAP(4, x, y) = 1;
            MAP(5, x, y) = 1;
        } else {
            MAP(5, x, y) = 1;
        }
        if (dirmode != 0) MAP(5, x, y) = 1;
        n++;
        flag = VuRandInt(3) - 1;
        ev[a->gimmickCount].type = VuRandInt(rng) + base + (dir << 6);
        ev[a->gimmickCount].x = x;
        ev[a->gimmickCount].flag = flag;
        ev[a->gimmickCount].y = y;
        a->gimmickCount++;
    }
}
