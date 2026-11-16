#include "common.h"
#include "field/n21.h"

void AmapSpecialRoomsCount(AmapWork *m) {
    s32 x;
    s32 y;
    s32 c;

    if (m->mapType == 2) {
        return;
    }
    for (y = 0; y < m->height; y++) {
        for (x = 0; x < m->width; x++) {
            c = m->tiles[y * m->width + x] & 0x1F;
            if (c >= 0x1B && c < 0x1E) {
                m->result = c | 0x80;
                m->specialRooms++;
            }
        }
    }
}
