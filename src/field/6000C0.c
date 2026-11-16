#include "common.h"
#include "protos/field.h"
#include "field/n20.h"

void MemMove128(u8 *, u8 *, s32);

void MapMiniLoad(MapMiniHdr *hdr) {
    u32 i;
    MapMiniTex *sb;
    u8 *v;

    if (hdr == NULL) {
        return;
    }
    mapMiniWork->hdr = hdr;
    mapMiniWork->floorCount = hdr->floorCount;
    mapMiniWork->texFloor = 0;
    mapMiniWork->floor = 0;
    for (i = 0; i < mapMiniWork->hdr->floorCount; i++) {
        if (mapMiniWork->hdr->tex[i] != 0) {
            mapMiniWork->hdr->tex[i] = (MapMiniTex *)((u32)mapMiniWork->hdr->tex[i] + (u32)mapMiniWork->hdr);
            GphTexDataRelocate(mapMiniWork->hdr->tex[i]);
            sb = mapMiniWork->hdr->tex[i];
            mapMiniWork->texSrc[i] = sb->pixels;
        }
        mapMiniWork->floorMinY[i] = mapMiniWork->hdr->floors[i].minY;
    }
    v = mapMiniWork->clut;
    sb = mapMiniWork->hdr->tex[0];
    MemMove128(v, sb->clut, 4);
    MapMiniTexRevealExplored();
}
