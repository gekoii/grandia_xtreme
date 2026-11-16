#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapFloodFill(s32 x, s32 y, AmapWork *a);

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapVoidFillEnclosed(AmapWork *a) {
    s32 x;
    s32 y;
    s32 n;
    s32 cnt;

    if (a->mapNo == 0xD848 || a->mapNo == 0xD84C) return;
    AmapLayerClear(3);
    n = a->voidFillMode;
    if (n == 0) return;
    switch (n) {
    case 1:
        AmapFloodFillEmpty(0, 0, a);
        cnt = 0;
        n = VuRandInt(7) + 1;
        for (y = 0; y < a->height && cnt < n; y++) {
            for (x = 0; x < a->width && cnt < n; x++) {
                if (MAP(3, x, y) == 0) {
                    AmapFloodFill(x, y, a);
                    cnt++;
                    break;
                }
            }
        }
        break;
    case 2:
        AmapFloodFillEmpty(0, 0, a);
        break;
    }
    for (y = 0; y < a->height; y++) {
        for (x = 0; x < a->width; x++) {
            if (MAP(3, x, y) == 0 && MAP(2, x, y) != 1 && AmapCellIsJoint(x, y, a) == 0) {
                fldMapCells[y * fldMapWidth + x] = 2;
                MAP(6, x, y) = 1;
            }
        }
    }
}
