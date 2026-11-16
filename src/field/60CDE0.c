#include "common.h"

extern s32 fldMapLayerSize;
extern u8 *fldMapCells;
extern s32 fldMapWidth;

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

s32 AmapAlcoveSpotFind(s16 x, s16 y) {
    s32 d;
    s32 i;
    s32 j;
    s32 f;

    d = -1;
    f = -1;
    if (MAP(0, x, y - 1) == 1) d = 0;
    if (MAP(0, x + 1, y) == 1) d = 1;
    if (MAP(0, x, y + 1) == 1) d = 2;
    if (MAP(0, x - 1, y) == 1) d = 3;
    if (d == -1) return -1;
    for (j = -1; j < 2; j++) {
        for (i = -1; i < 2; i++) {
            if (MAP(4, x + i, y + j) != 0) goto fail; /* gotos to one shared return -1 (separate returns add insns) */
        }
    }
    f = 0;
    switch (d) {
    case 0:
        for (j = 0; j < 4; j++) {
            for (i = -2; i < 3; i++) {
                if (MAP(0, x + i, y + j) != 0) f = 1;
                if (MAP(1, x + i, y + j) != 0) f = 1;
                if (MAP(2, x + i, y + j) != 0) f = 1;
                if (f) goto fail;
            }
        }
        break;
    case 1:
        for (i = -3; i < 1; i++) {
            for (j = -2; j < 3; j++) {
                if (MAP(0, x + i, y + j) != 0) f = 1;
                if (MAP(1, x + i, y + j) != 0) f = 1;
                if (MAP(2, x + i, y + j) != 0) f = 1;
                if (f) goto fail;
            }
        }
        break;
    case 2:
        for (j = -3; j < 1; j++) {
            for (i = -2; i < 3; i++) {
                if (MAP(0, x + i, y + j) != 0) f = 1;
                if (MAP(1, x + i, y + j) != 0) f = 1;
                if (MAP(2, x + i, y + j) != 0) f = 1;
                if (f) goto fail;
            }
        }
        break;
    case 3:
        for (i = 0; i < 4; i++) {
            for (j = -2; j < 3; j++) {
                if (MAP(0, x + i, y + j) != 0) f = 1;
                if (MAP(1, x + i, y + j) != 0) f = 1;
                if (MAP(2, x + i, y + j) != 0) f = 1;
                if (f) goto fail;
            }
        }
        break;
    }
    if (f != 0) {
fail:
        return -1;
    }
    return d;
}
