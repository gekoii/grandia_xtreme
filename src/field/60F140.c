#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapDistVisitFloor(s32 x, s32 y, s32 v, AmapWork *a) {
    AmapPtList *l = a->ptList;

    if (MAP(0, x, y) == 1 && v < MAP(3, x, y)) {
        l->pts[l->count].x = x;
        l->pts[l->count].y = y;
        l->count++;
        MAP(3, x, y) = v;
    }
}
