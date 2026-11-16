/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n22.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void VuBezier3(FVECTOR *, FVECTOR *, f32);

void MapNearObjUpdate(void) {
    FVECTOR pts[4];
    ChrObj *pc;
    GphPart *o;
    void *h;
    u32 i;
    s32 id;
    s32 grp;
    s32 cnt;

    pc = ChrGetByNo(1);
    if (pc == NULL) {
        return;
    }
    for (i = 0; i < mapNearObjTable->count; i++) {
        mapNearObjTable->objs[i].dist = FldVecDist(mapNearObjTable->objs[i].partPos, pc->model->pos);
        grp = mapNearObjTable->objs[i].group;
        id = mapNearObjTable->objs[i].inst;
        cnt = ChrInstGetBlink(grp, id);
        h = ChrInstGetModel(grp, id);
        if (ChrInstIsVisible(grp, id) == 1) {
            o = ChrModelFindPart(h, 1);
            if (o != NULL) {
                copyq(&pts[0], &mapNearObjTable->objs[i].pos);
                copyq(&pts[1], &mapNearObjTable->objs[i].pos);
                copyq(&pts[2], &mapNearObjTable->objs[i].pos);
                copyq(&pts[3], &mapNearObjTable->objs[i].pos);
                pts[1][1] += 40.0f;
                pts[2][1] += 40.0f;
                VuBezier3(&o->pos, pts, mapNearObjTable->objs[i].t);
                mapNearObjTable->objs[i].t += 0.01f;
                if (mapNearObjTable->objs[i].t > 1.0f) {
                    mapNearObjTable->objs[i].t = 0.0f;
                }
            }
        }
        if (mapNearObjTable->objs[i].state == 0) {
            mapNearObjTable->objs[i].dist = 0.0f;
            mapNearObjTable->objs[i].alpha -= 3.0f;
            ChrModelSetAlpha(h, mapNearObjTable->objs[i].alpha);
            mapNearObjTable->objs[i].fading = 1;
            if (mapNearObjTable->objs[i].alpha < -127.0f) {
                ChrInstSetVisible(grp, id, 0);
            }
        } else {
            if (FldInputLockGet() == 1) {
                mapNearObjTable->objs[i].dist = 90000.0f;
            }
            if (mapNearObjTable->objs[i].dist < mapNearObjTable->objs[i].range) {
                if (ChrInstIsVisible(grp, id) == 1) {
                    if (mapNearObjTable->objs[i].dist < 100.0f) {
                        mapNearObjTable->objs[i].alpha -= 3.0f;
                        ChrModelSetAlpha(h, mapNearObjTable->objs[i].alpha);
                        mapNearObjTable->objs[i].fading = 1;
                        if (mapNearObjTable->objs[i].alpha < -127.0f) {
                            ChrInstSetVisible(grp, id, 0);
                        }
                    } else if (mapNearObjTable->objs[i].alpha < 0.0f) {
                        mapNearObjTable->objs[i].alpha += 3.0f;
                        if (mapNearObjTable->objs[i].alpha > 0.0f) {
                            mapNearObjTable->objs[i].alpha = 0.0f;
                        }
                        ChrModelSetAlpha(h, mapNearObjTable->objs[i].alpha);
                    }
                } else if (mapNearObjTable->objs[i].fading == 0) {
                    FldSndSePlay(3, 0x7D6, 0);
                    ChrInstSetVisible(grp, id, 1);
                    ChrInstSetBlink(grp, id, 0x1E);
                    mapNearObjTable->objs[i].alpha = 0.0f;
                    ChrModelSetAlpha(h, mapNearObjTable->objs[i].alpha);
                }
            } else {
                mapNearObjTable->objs[i].fading = 0;
                if (ChrInstIsVisible(grp, id) == 1 && cnt == 0) {
                    mapNearObjTable->objs[i].alpha -= 2.0f;
                    ChrModelSetAlpha(h, mapNearObjTable->objs[i].alpha);
                    if (mapNearObjTable->objs[i].alpha < -127.0f) {
                        ChrInstSetVisible(grp, id, 0);
                    }
                }
            }
            if (cnt != 0) {
                cnt--;
                ChrInstSetBlink(grp, id, cnt);
            }
        }
    }
}
