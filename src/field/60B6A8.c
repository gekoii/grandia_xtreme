#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapVoidCarveWallSide(AmapWork *m) {
    s32 x;
    s32 y;
    s32 tries;
    s32 cnt;
    s32 n;
    s32 k;
    s32 all;

    AmapLayerClear(3);
    cnt = 0;
    tries = 0;
    while (1) { /* goto next: the shared loop test (a do-while with continue differs) */
        x = VuRandInt(m->width - 2) + 1;
        y = VuRandInt(m->height - 2) + 1;
        if (m->startX == x && m->startY == y) goto next;
        if (m->exitX == x && m->exitY == y) goto next;
        if (MAP(0, x, y) != 1) goto next;
        if (MAP(1, x, y) == 1) goto next;
        if (MAP(4, x, y) != 0) goto next;
        n = 0;
        k = 0;
        if (MAP(0, x, y - 1) == 0) n |= 1;
        if (MAP(0, x, y - 1) == 2) k |= 1;
        if (MAP(0, x + 1, y - 1) == 0) n |= 2;
        if (MAP(0, x + 1, y - 1) == 2) k |= 2;
        if (MAP(0, x + 1, y) == 0) n |= 4;
        if (MAP(0, x + 1, y) == 2) k |= 4;
        if (MAP(0, x + 1, y + 1) == 0) n |= 8;
        if (MAP(0, x + 1, y + 1) == 2) k |= 8;
        if (MAP(0, x, y + 1) == 0) n |= 0x10;
        if (MAP(0, x, y + 1) == 2) k |= 0x10;
        if (MAP(0, x - 1, y + 1) == 0) n |= 0x20;
        if (MAP(0, x - 1, y + 1) == 2) k |= 0x20;
        if (MAP(0, x - 1, y) == 0) n |= 0x40;
        if (MAP(0, x - 1, y) == 2) k |= 0x40;
        if (MAP(0, x - 1, y - 1) == 0) n |= 0x80;
        if (MAP(0, x - 1, y - 1) == 2) k |= 0x80;
        all = n | k;
        if (k != 0) goto next;
        if (!(n & 0x55)) goto next;
        if ((n & 1) && (all & 0x7C)) goto next;
        if ((n & 4) && (all & 0xF1)) goto next;
        if ((n & 0x10) && (all & 0xF8)) goto next;
        if ((n & 0x40) && (all & 0x1F)) goto next;
        MAP(0, x, y) = 2;
        cnt++;
        MAP(4, x, y) = 1;
        MAP(6, x, y) = 3;
    next:
        if (++tries >= 0x100 || cnt >= 12) break;
    }
}
