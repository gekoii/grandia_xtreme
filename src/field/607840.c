#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapTilesBuild(AmapWork *m, u8 flag) {
    s32 a;
    s32 b;
    s32 x;
    s32 y;
    s32 v;
    s32 t;

    for (y = 0; y < m->height - 1; y++) {
        for (x = 0; x < m->width - 1; x++) {
            AmapCornerMask(x, y, &a, &b, m);
            a |= b << 4;
            v = amapCornerTileTable[a];
            if (flag) {
                if ((v & 0x3F) == 0) {
                    t = VuRandInt(100);
                    v &= 0xC0;
                    do { /* empty do-while(0): orders the ori before the slti and gives the alignment nop at the else label */
                    } while (0);
                    if (t + 1 >= 75) v |= 0x1E;
                }
            } else {
                if (MAP(1, x, y) != 1 && (v & 0x3F) == 0) {
                    v = (v & 0xC0) + 0x1E;
                }
            }
            if (MAP(1, x, y) == 1 && (v & 0x3F) == 0x3F) {
                v = (v & 0xC0) + 0x1F;
            }
            m->tiles[y * m->width + x] = v;
        }
        m->tiles[y * m->width + x] = 0x3F;
    }
    for (x = 0; x < m->width; x++) {
        m->tiles[(m->height - 1) * m->width + x] = 0x3F;
    }
}
