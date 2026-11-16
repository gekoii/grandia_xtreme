#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]


#define PLACE(k)                                                    \
    for (;;) {                                                      \
        x = VuRandInt(a->width - 2) + 1;                            \
        y = VuRandInt(a->height - 2) + 1;                            \
        if (MAP(3, x, y) != ff && MAP(4, x, y) == 0) {            \
            ev[a->gimmickCount].type = (d << 6) | (k);                          \
            ev[a->gimmickCount].x = x;                                       \
            ev[a->gimmickCount].y = y;                                       \
            MAP(4, x, y) = 1;                                       \
            a->gimmickCount++;                                               \
            break;                                                  \
        }                                                           \
    }

void AmapGateSwitchesPlace(AmapWork *a) {
    AmapPlace *ev = a->gimmicks;
    s32 i;
    s32 n = 0;
    s32 d;
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    s32 ff = 0xFF;

    for (i = 0; i < a->gimmickCount; i++) {
        if ((ev[i].type & 0x1F) == 4) {
            n = i + 1;
            break;
        }
    }
    if (n == 0) {
        return;
    }
    AmapLayerClear(3);
    n--;
    d = ev[n].type >> 6;
    py = ev[n].y;
    px = ev[n].x;
    switch (d) {
    case 0:
        py--;
        break;
    case 1:
        px++;
        break;
    case 2:
        py++;
        break;
    case 3:
        px--;
        break;
    }
    MAP(0, ev[n].x, ev[n].y) = 3;
    AmapStartReachCheck(px, py, a);
    PLACE(5)
    PLACE(6)
    PLACE(7)
    PLACE(8)
}
