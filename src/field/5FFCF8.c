#include "common.h"
#include "field/n20.h"
#include "protos/field.h"

void MapMiniTexRevealExplored(void) {
    u32 i;
    s32 y;
    s32 x;
    s32 r;
    s32 k;
    s32 id;
    u8 *dst;
    u8 *src;
    s32 off;
    s32 stride;

    id = EvtGetMapNo();
    for (i = 0; i < mapMiniWork->hdr->floorCount; i++) {
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 16; x++) {
                if (MapMiniExploredGet(MapMiniIndexGet(id) + i, y * 16 + x)) {
                    dst = mapMiniWork->texBufs[i];
                    src = mapMiniWork->texSrc[i];
                    if (dst != NULL && src != NULL) {
                        stride = 0x80;
                        off = ((u8)(y * 16) << 8 | (u8)(x * 16)) >> 1;
                        for (r = 0; r < 16; r++) {
                            for (k = 0; k < 8; k++) {
                                dst[stride * r + off + k] = src[stride * r + off + k];
                            }
                        }
                    }
                }
            }
        }
    }
}
