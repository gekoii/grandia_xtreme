#include "common.h"
#include "field/n21.h"

void AmapTileCountsBuild(AmapWork *m) {
    s32 i;
    s32 c;

    for (i = 0; i < 64; i++) {
        m->tileCounts[i] = 0;
    }
    for (i = 0; i < m->width * m->height; i++) {
        c = m->tiles[i];
        if (c == -1) {
            m->tileCounts[63]++;
        } else {
            m->tileCounts[c & 0x3F]++;
        }
    }
}
