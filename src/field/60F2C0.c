#include "common.h"

typedef struct {
    s32 n;
    u32 max;
    struct {
        s8 x;
        s8 y;
    } e[1];
} PtList;

typedef struct {
    u8 pad[0x13];
    u8 w;
    u8 h;
    u8 pad15[0x178 - 0x15];
    PtList *list;
} Area;

extern s32 fldMapLayerSize;
extern u8 *fldMapCells;
extern s32 fldMapWidth;

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapFloodFill(s32 x, s32 y, Area *a) {
    PtList *l = a->list;
    s32 dir;
    s8 x0;
    s32 up;
    s32 down;
    u8 *p;
    s32 idx;

    l->e[0].x = x;
    l->e[0].y = y;
    l->n = 1;
    do {
        l->n--;
        down = 0;
        up = 0;
        x = l->e[l->n].x;
        y = l->e[l->n].y;
        dir = 1;
        x0 = x;
        for (;;) {
            if (x >= a->w) {
                dir = -1;
                x = x0 - 1;
                up = 0;
                down = 0;
            }
            if (y >= a->h || x < 0) break;
            idx = fldMapLayerSize * 3 + x + y * fldMapWidth;
            p = &fldMapCells[idx];
            if (*p == 0) {
                *p = 1;
                if (y < a->h) {
                    if (MAP(3, x, y + 1) == 0) {
                        if (down == 0) {
                            down = 1;
                            l->e[l->n].x = x;
                            l->e[l->n].y = y + 1;
                            l->n++;
                        }
                    } else if (down == 1) {
                        down = 0;
                    }
                }
                if (y != 0) {
                    if (MAP(3, x, y - 1) == 0) {
                        if (up == 0) {
                            up = 1;
                            l->e[l->n].x = x;
                            l->e[l->n].y = y - 1;
                            l->n++;
                        }
                    } else if (up == 1) {
                        up = 0;
                    }
                }
            } else {
                if (dir != 1) break;
                x = x0;
                dir = -1;
                up = 0;
                down = 0;
            }
            x += dir;
        }
    } while (l->n != 0);
}
