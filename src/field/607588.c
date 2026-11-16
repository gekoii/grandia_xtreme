#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapFloorFixGaps(AmapWork *m) {
    s32 a;
    s32 b;
    s32 x;
    s32 y;
    s32 n;

    for (y = 1; y < m->height - 1; y++) {
        for (x = 1; x < m->width - 1; x++) {
            if (MAP(0, x, y) == 0) {
                n = 0;
                if (MAP(0, x, y - 1) == 1) n++;
                if (MAP(0, x + 1, y) == 1) n++;
                if (MAP(0, x, y + 1) == 1) n++;
                if (MAP(0, x - 1, y) == 1) n++;
                if (n == 4) {
                    MAP(0, x, y) = 1;
                    MAP(1, x, y) = 2;
                    MAP(2, x, y) = 2;
                }
            }
            AmapCornerMask(x, y, &a, &b, m);
            a |= b << 4;
            if (a == 10) {
                MAP(0, x, y + 1) = 1;
                MAP(1, x, y + 1) = 2;
                MAP(2, x, y + 1) = 2;
            }
            if (a == 5) {
                MAP(0, x + 1, y + 1) = 1;
                fldMapCells[x + (fldMapLayerSize * 1 + (y + 1) * fldMapWidth) + 1] = 2;
                fldMapCells[x + (fldMapLayerSize * 2 + (y + 1) * fldMapWidth) + 1] = 2;
            }
        }
    }
}
