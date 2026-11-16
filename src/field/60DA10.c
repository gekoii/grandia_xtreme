#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapDeadEndItemsPlace(AmapWork *a) {
    AmapItemPlace *ev;
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    s32 v;
    s32 k;
    AmapNode *p;

    if (amapNodeCount != 0) {
        ev = a->items;
        i = 0;
        n = a->itemCount;
        for (; i < amapNodeCount && n < a->itemMax; i++) {
            p = (AmapNode *)(i * 6 + (u32)amapNodes); /* &amapNodes[i], index added before the base */
            x = p->x;
            y = p->y;
            if (MAP(1, x, y) != 1 && MAP(4, x, y) == 0) {
                v = p->floorMask;
                k = 0;
                if (v == 1) k = 1;
                if (v == 4) k = 1;
                if (v == 0x10) k = 1;
                if (v == 0x40) k = 1;
                if ((v & 0x77) == 0x70) k = 1;
                if ((v & 0xDD) == 0x1C) k = 1;
                if ((v & 0xDD) == 0xC1) k = 1;
                if ((v & 0x77) == 7) k = 1;
                if (k) {
                    ev[n].type = 1;
                    ev[n].x = x;
                    ev[n].y = y;
                    MAP(4, x, y) = 1;
                    ev[n].facing &= 0x3F;
                    if (v & 1) {
                        ev[n].facing |= 0x80;
                    } else if (v & 4) {
                        ev[n].facing |= 0x40;
                    } else if (v & 0x10) {
                        ev[n].facing |= 0;
                    } else if (v & 0x40) {
                        ev[n].facing |= 0xC0;
                    }
                    n++;
                }
            }
        }
        a->itemCount = (u8)n;
    }
}
