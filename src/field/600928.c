/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

void GphModelUpdateGsRegs(s32);

void MapMiniTexUpdate(void) {
    ChrObj *c;
    GphModel *o;
    s32 base;
    s32 sbase;
    s32 dst;
    s32 src;
    s32 w;
    s32 idx;
    s32 off;
    s32 y;
    s32 k;

    if (MapMiniIsHidden(0) != 0) {
        return;
    }
    c = ChrGetByNo(1);
    if (c == NULL) {
        return;
    }
    MapMiniExploreAt(c->model->pos);
    GphModelUpdateGsRegs(mapMiniWork->model);
    if (mapMiniWork->floor != mapMiniWork->texFloor) {
        mapMiniWork->texFloor = mapMiniWork->floor;
        mapMiniPac->tex->pixels = mapMiniWork->texBufs[mapMiniWork->texFloor];
    }
    base = (s32)mapMiniWork->texBufs[mapMiniWork->texFloor];
    sbase = (s32)mapMiniWork->texSrc[mapMiniWork->texFloor];
    if (base == 0 || sbase == 0) {
        return;
    }
    o = c->model;
    idx = ((u8)(5 - (s32)((o->pos[2] + 16000.0f) / 125.0f)) << 8 | (u8)(s32)((o->pos[0] + 16000.0f) / 125.0f)) >> 1;
    w = 128;
    for (y = 0; y < 18; y++) {
        off = w * y + 6;
        off = idx - off;
        src = off + sbase;
        dst = off + base;
        for (k = 11; k >= 0; k--) {
            if (dst >= base && dst < base + 0x7FFF) {
                *(u8 *)dst = *(u8 *)src;
            }
            src++;
            dst++;
        }
    }
}
