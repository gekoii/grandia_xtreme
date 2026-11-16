#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapTilesSetAround(AmapWork *m, s32 x, s32 y) {
    s32 a;
    s32 b;
    s32 i;
    s32 j;
    u8 v;

    for (j = y - 1; j <= y; j++) {
        for (i = x - 1; i <= x; i++) {
            AmapCornerMask(i, j, &a, &b, m);
            a |= b << 4;
            v = amapCornerTileTable[a];
            if ((v & 0x3F) == 1 && VuRandInt(2) == 0) {
                v &= 0xC0; v |= 4;
            }
            if (m->mapType == 2 && (v & 0x3F) == 0xB) {
                v &= 0xC0; v |= 0x1A;
            }
            m->tiles[j * m->width + i] = v;
        }
    }
}
