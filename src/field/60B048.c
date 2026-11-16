#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapBranchWalk(s32 x, s32 y, s32 dir, AmapWork *a) {
    s32 done;
    s32 dx;
    s32 dy;
    s32 k;
    s32 len;
    s32 r;

    dx = 0;
    done = 0;
    dy = 0;
    do {
        r = VuRandInt(1);
        if (r == 0) r = -1;
        dir = (dir + r) & 3;
        switch (dir) {
        case 0:
            dx = 0;
            dy = -1;
            break;
        case 1:
            dx = 1;
            dy = 0;
            break;
        case 2:
            dx = 0;
            dy = 1;
            break;
        case 3:
            dx = -1;
            dy = 0;
            break;
        }
        len = VuRandInt(5) + 1;
        k = 0;
        while (k < len) {
            k++;
            x += dx;
            y += dy;
            if (x >= a->width - 1 || x <= 0 || y >= a->height - 1 || y <= 0) {
                do { /* both do-while(0): register allocation (they double the undo steps' weight) */
                    x -= dx;
                } while (0);
                do {
                    y -= dy;
                } while (0);
                break;
            }
            done = 1;
            if (MAP(2, x, y) != 0) break;
            done = 0;
            AmapCellSetBranch(x, y);
        }
    } while (done == 0);
}
