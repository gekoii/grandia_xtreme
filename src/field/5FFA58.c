#include "common.h"

extern u16 mapMiniMapIds[];

s32 MapMiniIndexGet(u16 id) {
    s32 i;
    u16 *p;

    if ((id & 0xFF00) == 0xA000 || (id & 0xFF00) == 0x6000 || (id & 0xFF00) == 0xD800) {
        return 0x2F;
    }
    if (id == 0x5824) {
        id = 0x5820;
    }
    p = mapMiniMapIds;
    for (i = 0; i < 0x30; i++) {
        if (*p == id) {
            return i;
        }
        p++;
    }
    return -1;
}
