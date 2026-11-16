#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapNodesScatter(s32 n) {
    s32 i;
    s32 w;
    s32 h;
    s32 x;
    s32 y;

    i = 0;
    w = amapBoundX1 - amapBoundX0;
    h = amapBoundY1 - amapBoundY0;
    do {
        x = VuRandInt(w);
        y = VuRandInt(h) + amapBoundY0;
        x += amapBoundX0;
        if (MAP(2, x, y) == 0) {
            AmapCellSetNode(x, y);
            amapNodes[amapNodeCount].x = x;
            amapNodes[amapNodeCount].y = y;
            amapNodeCount++;
        }
    } while (amapNodeCount < n && i++ < 0x401);
}
