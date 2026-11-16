/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n13.h"
#include "field/n14.h"
#include "protos/field.h"

/* A ChrObj (field/state.h) with the wall hit list at +0x228: 400 entries run
 * past ChrObj's 0x730 bytes, and reaching them through ChrObj.unk228 changes
 * the code, so this file keeps its own view. */
typedef struct {
    u8 pad0[8];
    s32 flags;
    u8 padC[0x50];
    s8 modelIdx;
    u8 pad5D[0x103];
    struct ChrTenantArea *models[8];
    u8 pad180[0xA4];
    s16 nHits;
    u8 pad226[2];
    u32 hits[400];
} Actor5C06;

void MapWallCollectInArea(Actor5C06 *a) {
    f32 box[4];
    ChrTenantArea *m;
    MapWall *ar;
    s32 n, i, j, k, hit;

    if (a == NULL) {
        return;
    }
    if (a->models[a->modelIdx] == NULL || (a->flags & 0x200)) {
        a->nHits = 0xFF;
        return;
    }
    MapWallSelectLayer(0);
    m = a->models[a->modelIdx];
    if (m->x0 <= m->x1) {
        box[0] = m->x0;
        box[2] = m->x1;
    } else {
        box[0] = m->x1;
        box[2] = m->x0;
    }
    m = a->models[a->modelIdx];
    if (m->z0 <= m->z1) {
        box[1] = m->z0;
        box[3] = m->z1;
    } else {
        box[1] = m->z1;
        box[3] = m->z0;
    }
    n = MapWallGetCount();
    k = 0;
    for (i = 0; i < n; i++) {
        ar = (MapWall *)MapWallGet(i);
        for (j = 0; j < ar->count - 1; j++) {
            hit = 0;
            if (box[0] < ((MapWall *)((u32)ar + (j << 4)))->points[0][0] && ((MapWall *)((u32)ar + (j << 4)))->points[0][0] < box[2] && box[1] < ((MapWall *)((u32)ar + (j << 4)))->points[0][2] && ((MapWall *)((u32)ar + (j << 4)))->points[0][2] < box[3]) {
                hit = 1;
            }
            if (box[0] < ((MapWall *)((u32)ar + ((j + 1) << 4)))->points[0][0] && ((MapWall *)((u32)ar + ((j + 1) << 4)))->points[0][0] < box[2] && box[1] < ((MapWall *)((u32)ar + ((j + 1) << 4)))->points[0][2] && ((MapWall *)((u32)ar + ((j + 1) << 4)))->points[0][2] < box[3]) {
                hit = 1;
            }
            if (hit == 1) {
                a->hits[k++] = (u32)&ar->points[j];
                a->hits[k++] = (u32)&ar->points[j + 1];
                if (ar->attr.i != 0) {
                    a->hits[k++] = i;
                } else {
                    a->hits[k++] = 0xFFFFFFFF;
                }
                if (k >= 400) {
                    return;
                }
            }
        }
    }
    a->nHits = k / 3;
}
