#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapDistVisitFree(s32 x, s32 y, s32 v, AmapWork *a) {
    AmapPtList *l = a->ptList;

    if (x < a->width - 1 && x > 0 && y < a->height - 1 && y > 0) {
        if (MAP(2, x, y) == 0 && v < MAP(3, x, y)) {
            l->pts[l->count].x = x;
            l->pts[l->count].y = y;
            l->count++;
            MAP(3, x, y) = v;
        }
    }
}
