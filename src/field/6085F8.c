#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapExitPlace(AmapWork *o) {
    s32 bx;
    s32 by;
    s32 best;
    s32 rw;
    s32 rh;
    s32 x;
    s32 y;
    s32 n;
    s32 i;
    s32 j;
    s32 hit;
    s32 d;
    u8 *p;
    s32 xm;
    s32 k;
    u8 *m;
    s32 b;
    s32 one;

    if (o->exitFixed) {
        o->exitX = o->exitFixedX;
        o->exitY = o->exitFixedY;
        MAP(4, o->exitX, o->exitY) = 0xFF;
        MAP(5, o->exitX, o->exitY) = 0xFF;
        return;
    }
    one = 1;
    rw = amapBoundX1 - amapBoundX0;
    best = 0;
    by = 0;
    bx = 0;
    n = 0;
    rh = amapBoundY1 - amapBoundY0;
    /* goto loop shape and the three do-while(0) blocks are the original's layout:
     * each block unwrapped, or a structured loop, changes the code */
    while (1) {
        do {
            x = VuRandInt(rw) + amapBoundX0;
            y = VuRandInt(rh) + amapBoundY0;
            hit = 0;
            for (j = -1; j < 2; j++) {
                k = (y + j) * fldMapWidth;
                m = fldMapCells;
                b = fldMapLayerSize;
                xm = x - 1;
                p = (u8 *)((xm + (k + b)) + (u32)m);
                for (i = 0; i < 3; i++) {
                    if (*p == 1) hit = one;
                    if (*p == 3) hit = one;
                    p++;
                }
            }
        } while (0);
        if (hit == 0) {
            d = AmapGridDist(x, y, o->startX, o->startY);
            if (d >= 17) goto found;
            if (d >= best) {
                best = d;
                bx = x;
                by = y;
            }
        }
        if (++n < 257) continue;
        x = bx;
        do {
            y = by;
        found:
            o->exitX = x;
            o->exitY = y;
            n = 0;
            if (MAP(2, x, y) == 0) goto out;
        } while (0);
    }
out:
    do {
        AmapCellSetNode(x, y);
        MAP(4, x, y) = 0xFF;
        MAP(5, x, y) = 0xFF;
    } while (0);
    AmapNodeAdd(o->exitX, o->exitY);
}
