#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapStartPlace(AmapWork *o) {
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

    if (o->startFixed) {
        o->startX = o->startFixedX;
        o->startY = o->startFixedY;
        MAP(4, o->startX, o->startY) = 0xFF;
        MAP(5, o->startX, o->startY) = 0xFF;
        return;
    }
    best = 0;
    by = 0;
    rw = amapBoundX1 - amapBoundX0;
    bx = 0;
    rh = amapBoundY1 - amapBoundY0;
    if (o->exitFixed) {
        do {
            n = 0;
            do {
                n++;
                x = VuRandInt(rw) + amapBoundX0;
                y = VuRandInt(rh) + amapBoundY0;
                xm = x - 1;
                hit = 0;
                for (j = -1; j < 2; j++) {
                    k = (y + j) * fldMapWidth;
                    m = fldMapCells;
                    b = fldMapLayerSize;
                    p = (u8 *)((xm + (k + b)) + (u32)m);
                    for (i = 0; i < 3; i++) {
                        if (*p == 1) hit = 1;
                        if (*p == 3) hit = 1;
                        p++;
                    }
                }
                if (hit == 0) {
                    d = AmapGridDist(x, y, o->exitFixedX, o->exitFixedY);
                    if (d >= 17) goto found; /* goto: keeps this x, y (skips the fallback to the best one) */
                    if (d >= best) {
                        best = d;
                        bx = x;
                        by = y;
                    }
                }
            } while (n < 257);
            x = bx;
            y = by;
        found:;
        } while (MAP(2, x, y) != 0);
    } else {
        do {
            x = VuRandInt(rw) + amapBoundX0;
            y = VuRandInt(rh) + amapBoundY0;
            xm = x - 1;
            hit = 0;
            for (j = -1; j < 2; j++) {
                k = (y + j) * fldMapWidth;
                m = fldMapCells;
                b = fldMapLayerSize;
                p = (u8 *)((xm + (k + b)) + (u32)m);
                for (i = 0; i < 3; i++) {
                    if (*p == 1) hit = 1;
                    if (*p == 3) hit = 1;
                    p++;
                }
            }
        } while (hit || MAP(2, x, y) != 0);
    }
    o->startX = x;
    o->startY = y;
    AmapCellSetNode(x, y);
    MAP(4, x, y) = 0xFF;
    MAP(5, x, y) = 0xFF;
    AmapNodeAdd(o->startX, o->startY);
}
