#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]
#define TILE(x, y) m->tiles[(y) * m->width + (x)]

void AmapTilesRandomize(AmapWork *m) {
    s32 x;
    s32 y;
    s32 r;
    u8 v;

    for (y = 0; y < m->height - 1; y++) {
        for (x = 0; x < m->width - 1; x++) {
            if (MAP(1, x, y) == 1) continue;
            if (TILE(x, y) == 0x81 && TILE(x + 1, y) == 1 && VuRandInt(4) == 0) {
                TILE(x, y) |= 4;
                TILE(x + 1, y) |= 4;
            }
            if (TILE(x, y) == 0xC1 && TILE(x, y + 1) == 0x41 && VuRandInt(4) == 0) {
                TILE(x, y) |= 4;
                TILE(x, y + 1) |= 4;
            }
            v = TILE(x, y);
            if ((v & 0x3F) == 1) {
                v &= 0xC0;
                r = VuRandInt(100) + 1;
                do { /* do-while(0) + break: an if/else chain loses two insns of layout */
                    if (r < 25) { v |= 1; break; }
                    if (r < 50) { v |= 4; break; }
                    if (r < 75) { v |= 0x17; break; }
                    v |= 0x18;
                } while (0);
            }
            TILE(x, y) = v;
        }
    }
}
