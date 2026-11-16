#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]


s32 AmapStartReachCheck(s32 x, s32 y, AmapWork *a) {
    AmapPtList *l = a->ptList;
    s32 d;
    s32 r;
    s32 cx;
    s32 cy;

    l->pts[l->count].x = x;
    l->pts[l->count].y = y;
    MAP(3, x, y) = 0;
    l->count++;
    while (l->count <= 0x1000) {
        if (l->count == 0) break;
        if (l->max < l->count) {
            l->max = l->count;
        }
        l->count--;
        y = l->pts[l->count].y;
        x = l->pts[l->count].x;
        d = MAP(3, x, y) + 1;
        if (d < 0xFF) if (d != 0xFE) {
            AmapDistVisitFloor(x, y - 1, d, a);
            AmapDistVisitFloor(x + 1, y, d, a);
            AmapDistVisitFloor(x, y + 1, d, a);
            AmapDistVisitFloor(x - 1, y, d, a);
        }
    }
    cx = a->startX;
    cy = a->startY;
    r = 0;
    if (MAP(3, cx, cy) == 0xFF && fldMapCells[cx + (fldMapLayerSize * 3 + cy * fldMapWidth) + 1] == 0xFF && fldMapCells[cx + (fldMapLayerSize * 3 + cy * fldMapWidth) - 1] == 0xFF &&
        MAP(3, cx, cy - 1) == 0xFF && MAP(3, cx, cy + 1) == 0xFF) {
        r = -1;
    }
    return r;
}
