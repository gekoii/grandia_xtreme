#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]


void AmapDistFillFree(s32 x, s32 y, AmapWork *a) {
    AmapPtList *l = a->ptList;
    s32 d;

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
        if (d < 0x1C) {
            AmapDistVisitFree(x, y - 1, d, a);
            AmapDistVisitFree(x + 1, y, d, a);
            AmapDistVisitFree(x, y + 1, d, a);
            AmapDistVisitFree(x - 1, y, d, a);
        }
    }
}
