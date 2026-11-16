/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "save.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

void VuBezier3(FVECTOR *, FVECTOR *, f32);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void MapMiniUpdate(void) {
    FVECTOR v;
    FVECTOR pts[4];
    FVECTOR pos;
    ChrObj *ch;
    Camera *c;
    s32 one;

    if (MapMiniIsHidden(1) != 0) {
        return;
    }
    ch = ChrGetByNo(1);
    if (ch == NULL) {
        return;
    }
    c = &camField[1];
    c->mode = CAM_MODE_NORMAL;
    c->yaw = camField[0].yaw;
    c->roll = 0.0f;
    one = 1;
    MapMiniExploreAt(ch->model->pos);
    switch (sysSaveData.optMiniMapZoom) {
    case 0:
        mapMiniWork->zoomTo[1] = 90.0f;
        mapMiniWork->zoomTo[2] = 1.5f;
        break;
    case 1:
        mapMiniWork->zoomTo[1] = 160.0f;
        mapMiniWork->zoomTo[2] = 2.0f;
        break;
    }
    if (sysSaveData.optMiniMap2A == 1) {
        c->yaw = 0.0f;
    }
    if (sysSaveData.optMiniMap2B == 0) {
        mapMiniWork->zoomTo[0] = 0.6981317f;
    } else {
        mapMiniWork->zoomTo[0] = 1.5707964f;
    }
    mapMiniWork->zoomT += 0.02f;
    if (mapMiniWork->zoomT > 1.0f) {
        mapMiniWork->zoomT = 1.0f;
    }
    copyq(&pts[0], &mapMiniWork->zoomFrom);
    copyq(&pts[1], &mapMiniWork->zoomTo);
    copyq(&pts[2], &mapMiniWork->zoomTo);
    copyq(&pts[3], &mapMiniWork->zoomTo);
    VuBezier3(&v, pts, mapMiniWork->zoomT);
    c->pitch = v[0];
    c->dist = v[1];
    mapMiniWork->iconScale = v[2];
    if (padPort0[0].trigger & 0x80) {
        SndSePlay(0x7ED, 0, 0, 0x1E, 0);
        mapMiniWork->zoomT = 0.0f;
        mapMiniWork->zoomFrom[0] = v[0];
        mapMiniWork->zoomFrom[1] = v[1];
        mapMiniWork->zoomFrom[2] = v[2];
        sysSaveData.optMiniMapZoom ^= 1;
    }
    if (one == 1) {
        copyq(&pos, &ch->model->pos);
        c->lookAt[3] = 0.0f;
        c->lookAt[1] = 0.0f;
        c->lookAt[0] = pos[0] / 16000.0f * 64.0f;
        c->lookAt[2] = pos[2] / 16000.0f * 64.0f;
    } else {
        c->lookAt[0] = 0.0f;
        c->lookAt[1] = 0.0f;
        c->lookAt[2] = 0.0f;
        c->lookAt[3] = 0.0f;
    }
    ChrMotUpdate(mapMiniWork->model, 1.0f);
}
