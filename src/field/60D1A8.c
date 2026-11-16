#include "common.h"

extern s32 fldMapLayerSize;
extern u8 *fldMapCells;
extern s32 fldMapWidth;

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

s32 AmapAlcoveCarve(s16 x, s16 y, s16 d) {
    s32 i;
    s32 j;

    for (j = -1; j < 2; j++) {
        for (i = -1; i < 2; i++) {
            MAP(4, x + i, y + j) = 1;
            MAP(5, x + i, y + j) = 1;
        }
    }
    switch (d) {
    case 0:
        for (j = 1; j < 3; j++) {
            for (i = -1; i < 2; i++) {
                MAP(0, x + i, y + j) = 1;
                MAP(4, x + i, y + j) = 1;
                MAP(1, x + i, y + j) = 1;
                MAP(2, x + i, y + j) = 1;
            }
        }
        for (j = -1; j < 4; j++) {
            for (i = -2; i < 3; i++) {
                MAP(1, x + i, y + j) = 1;
                MAP(2, x + i, y + j) = 1;
            }
        }
        break;
    case 1:
        for (i = -2; i < 0; i++) {
            for (j = -1; j < 2; j++) {
                MAP(0, x + i, y + j) = 1;
                MAP(4, x + i, y + j) = 1;
            }
        }
        for (i = -3; i < 2; i++) {
            for (j = -2; j < 3; j++) {
                MAP(1, x + i, y + j) = 1;
                MAP(2, x + i, y + j) = 1;
            }
        }
        break;
    case 2:
        for (j = -2; j < 0; j++) {
            for (i = -1; i < 2; i++) {
                MAP(0, x + i, y + j) = 1;
                MAP(4, x + i, y + j) = 1;
            }
        }
        for (j = -3; j < 2; j++) {
            for (i = -2; i < 3; i++) {
                MAP(1, x + i, y + j) = 1;
                MAP(2, x + i, y + j) = 1;
            }
        }
        break;
    case 3:
        for (i = 1; i < 3; i++) {
            for (j = -1; j < 2; j++) {
                MAP(0, x + i, y + j) = 1;
                MAP(4, x + i, y + j) = 1;
            }
        }
        for (i = -1; i < 4; i++) {
            for (j = -2; j < 3; j++) {
                MAP(1, x + i, y + j) = 1;
                MAP(2, x + i, y + j) = 1;
            }
        }
        break;
    }
    return 0;
}
