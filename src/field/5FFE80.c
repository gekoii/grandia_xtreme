#include "common.h"
#include "grxsys.h"
#include "field/n20.h"
#include "protos/field.h"

typedef struct {
    u8 pad0[0x13];
    u8 x13;
    u8 x14;
    u8 pad15[0x120 - 0x15];
    u8 *x120;
} Map;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);
void MemMove128(u8 *, u8 *, s32);

void MapMiniBuildFromAmap(Map *map) {
    s32 y;
    s32 x;
    s32 r;
    s32 k;
    u8 c;
    s32 lo;
    s32 hi;
    s32 soff;
    s32 doff;
    s32 dx;
    s32 sstride;
    s32 dstride;
    u8 *dst;
    u8 *src;
    MapMiniTex *tex;

    dstride = 0x80;
    sstride = 0x20;
    mapMiniWork->hdr = &mapMiniAutoHdr;
    mapMiniWork->floorCount = 1;
    mapMiniWork->texFloor = 0;
    mapMiniWork->floor = 0;
    mapMiniWork->floorMinY[0] = 0.0f;
    mapMiniWork->floorMinY[1] = 999999.0f;
    mapMiniWork->floorMinY[2] = 999999.0f;
    mapMiniWork->texSrc[0] = FldMemAlloc(0x8000);
    MemSet(mapMiniWork->texSrc[0], 0, 0x8000);
    dst = mapMiniWork->texSrc[0];
    tex = (MapMiniTex *)sysGrxsysPac->tex08;
    src = tex->pixels;
    if (EvtGetMapNo() == 0xA008) {
        src += 0x400;
    }
    for (y = 0; y < map->x14; y++) {
        for (x = 0; x < map->x13; x++) {
            dx = (u8)(x * 4 + 0x7E);
            doff = ((u8)(y * 4 + 0x7C) << 8 | (u8)(x * 4 + 0x7E)) >> 1;
            r = map->x13 * y + x;
            c = map->x120[r];
            r = c & 0x3F;
            if (r < 0x20) {
                if (r >= 0x10) {
                    lo = (u8)((r - 0x10) * 4);
                    hi = (c >> 6) * 8 | 4;
                } else {
                    lo = r * 4;
                    hi = (c >> 6) * 8;
                }
                soff = (hi << 6 | lo) >> 1;
                for (r = 0; r < 4; r++) {
                    k = 0;
                    do {
                        dst[dstride * r + doff + k] = src[sstride * r + soff + k];
                    } while (++k < 2);
                }
            }
        }
    }
    MemMove128(mapMiniWork->clut, tex->clut, 4);
    MapMiniTexRevealExplored();
}
