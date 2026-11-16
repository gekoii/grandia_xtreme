#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapDistVisitOpen(s32 x, s32 y, s32 v, AmapWork *a) {
    AmapPtList *l = a->ptList;
    u8 c;

    if (x < a->width - 1 && x > 0 && y < a->height - 1 && y > 0) {
        c = MAP(2, x, y);
        if (c != 1 && c != 3 && v < MAP(3, x, y)) {
            l->pts[l->count].x = x;
            l->pts[l->count].y = y;
            l->count++;
            MAP(3, x, y) = v;
        }
    }
}
