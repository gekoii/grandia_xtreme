#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapType2WallsDecorate(AmapWork *a) {
    AmapPlace *ev = a->gimmicks;
    s32 y;
    s32 x;
    s32 t;
    u8 *p;

    if (a->mapType != 2) {
        return;
    }
    for (y = 0; y < a->height; y++) {
        for (x = 0; x < a->width; x++) {
            if (fldMapCells[y * fldMapWidth + x] == 0) {
                t = 0x11;
                if (VuRandInt(100) < 50) {
                    t = 0x12;
                }
                ev[a->gimmickCount].type = t;
                ev[a->gimmickCount].x = x;
                ev[a->gimmickCount].y = y;
                a->gimmickCount++;
            }
        }
    }
    for (y = 1; y < a->height - 1; y++) {
        for (x = 1; x < a->width - 1; x++) {
            p = &a->tiles[y * a->width + x];
            t = *p;
            if (t == 3 && a->tiles[(y - 1) * a->width + x] == 0xD) t = 0x19;
            if (t == 0x43 && a->tiles[(y - 1) * a->width + x] == 0x8C) t = 0x59;
            if (t == 0xC3 && a->tiles[(y + 1) * a->width + x] == 0xC) t = 0xD9;
            if (t == 0x83 && a->tiles[(y + 1) * a->width + x] == 0x8D) t = 0x99;
            if (t == 0x43 && p[1] == 0x4D) t = 0x59;
            if (t == 0x83 && p[1] == 0xCC) t = 0x99;
            if (t == 3 && p[-1] == 0x4C) t = 0x19;
            if (t == 0xC3 && p[-1] == 0xCD) t = 0xD9;
            *p = t;
        }
    }
}
