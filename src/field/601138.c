/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00674328-0x00674338 */
#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"
#include "field/n20.h"

typedef struct {
    s16 unk0;
    s16 unk2;
} Anim;

void GphQuad3DSetCorners(GphQuad3D *, f32 *, f32 *, f32, f32);

void MapMiniDraw(void) {
    s32 r[4];
    f32 m1[16];
    f32 m2[16];
    f32 vec[4];
    MapMiniWork *w;
    ChrObj *pl;
    ChrObj *e;
    GphModel *o;
    FldTreasure *base;
    FldTreasure *q;
    f32 *p;
    GphQuad3D *c;
    GphBatchQuad *qd;
    MapMiniWork *w0;
    double d;
    f32 a;
    f32 b;
    s32 i;

    if (MapMiniIsHidden(1) != 0) {
        return;
    }
    GsClearZBuffer();
    GphViewGetArea(6, r);
    GphBatchSetOffset(r[0], r[1]);
    w0 = mapMiniWork;
    qd = &w0->quads[0];
    w0->quads[0].type = 5;
    qd->v[0].r = 0;
    qd->v[0].g = 0x50;
    qd->v[0].b = 0x50;
    qd->v[0].a = 0x40;
    qd->v[1].r = 0;
    qd->v[1].g = 0x50;
    qd->v[1].b = 0x50;
    qd->v[1].a = 0x40;
    qd->v[2].r = 0;
    qd->v[2].g = 0x50;
    qd->v[2].b = 0x50;
    qd->v[2].a = 0x40;
    qd->v[3].r = 0;
    qd->v[3].g = 0x50;
    qd->v[3].b = 0x50;
    qd->v[3].a = 0x40;
    qd->v[0].x = 0;
    qd->v[0].y = 0;
    qd->v[1].x = 0;
    qd->v[1].y = r[3];
    qd->v[2].x = r[2];
    qd->v[2].y = 0;
    qd->v[3].x = r[2];
    qd->v[3].y = r[3];
    qd->z = 0;
    w0->quads[1].type = 0;
    GphBatchDrawQuads(&mapMiniWork->quads[0]);
    GphDrawListSet(fldDrawListMiniMapBuf);
    GphDrawListDrawAll();
    GsClearZBuffer();
    GphTexBind(0xEA);
    w = mapMiniWork;
    pl = ChrGetByNo(1);
    c = w->icons;
    VuMtxSetRotXY(m1, camField[1].pitch, camField[1].yaw, camField[0].roll);
    MapMiniArrowMatrix(m2, camField[1].pitch, camField[1].yaw, pl->heading * 9.587379924285257e-05f);
    if (sysSaveData.optMiniMapZoom == 0) {
        for (i = 0; i < ChrTenantGetCount(); i++) {
            e = ChrTenantAt(i);
            if (!(e->flags & 1)) continue;
            if (!(e->flags & 2)) continue;
            if (e->sense == NULL) continue;
            d = pl->model->pos[1] - (o = e->model)->pos[1];
            if (d < 0) d = -d;
            if (d > 2000.0) continue;
            if (!MapMiniIsExploredAt(o->pos)) continue;
            if (e->aiMode != 0) {
                mapMiniWork->iconSeen[i] = 1;
            }
            if (e->aiMode == 2) {
                mapMiniWork->iconAlpha[i] += 8;
                if ((s8)mapMiniWork->iconAlpha[i] >= 0) {
                    mapMiniWork->iconAlpha[i] = 0x80;
                }
            } else if (mapMiniWork->iconAlpha[i] != 0x80) {
                mapMiniWork->iconAlpha[i] -= 4;
                if ((s8)mapMiniWork->iconAlpha[i] >= 0) {
                    mapMiniWork->iconAlpha[i] = 0x80;
                }
            }
            if (mapMiniWork->iconSeen[i] == 0) continue;
            a = pl->model->pos[1];
            b = e->model->pos[1];
            if (b <= a) {
                if (b + 10000.0f < a) continue;
            } else {
                if (a + 10000.0f < b) continue;
            }
            c = MapMiniTenantIconAdd(c, e, m1, FldChrPackGetMapIcon((*(Anim **)e->sense)->unk2), mapMiniWork->iconAlpha[i]);
        }
        base = FldTreasureGetWork();
        if (base != NULL) {
            for (i = 0; i < 0x40; i++) {
                q = &base[i];
                if (q->state < 2) continue;
                d = pl->model->pos[1] - q->path[3][1];
                if (d < 0) d = -d;
                if (d > 2000.0) continue;
                if (!MapMiniIsExploredAt(q->path[3])) continue;
                c->col[0].r = 0x80;
                c->col[0].g = 0x80;
                c->col[0].b = 0x80;
                c->col[0].a = 0x80;
                c->col[1].r = 0x80;
                c->col[1].g = 0x80;
                c->col[1].b = 0x80;
                c->col[1].a = 0x80;
                c->col[2].r = 0x80;
                c->col[2].g = 0x80;
                c->col[2].b = 0x80;
                c->col[2].a = 0x80;
                c->col[3].r = 0x80;
                c->col[3].g = 0x80;
                c->col[3].b = 0x80;
                c->col[3].a = 0x80;
                c->type = 0xD;
                c->uv[0][0] = 0xB1;
                c->uv[0][1] = 0xDB;
                c->uv[1][0] = 0xB1;
                c->uv[1][1] = 0xEB;
                c->uv[2][0] = 0xC1;
                c->uv[2][1] = 0xDB;
                c->uv[3][0] = 0xC1;
                c->uv[3][1] = 0xEB;
                vec[0] = q->path[3][0] / 16000.0f * 62.5f;
                vec[1] = 0;
                vec[2] = q->path[3][2] / 16000.0f * 62.5f;
                vec[3] = 0;
                GphQuad3DSetCorners(c, m1, vec, mapMiniWork->iconScale * 0.5f, mapMiniWork->iconScale * 0.5f);
                c++;
            }
        }
    }
    if (AmapIsLoaded() == 1) {
        for (i = 0; i < 0x10; i++) {
            p = MapEntranceGet(i);
            if (p == NULL) break;
            if (!MapMiniIsExploredAt(p)) continue;
            c->col[0].r = 0x80;
            c->col[0].g = 0x80;
            c->col[0].b = 0x80;
            c->col[0].a = 0x80;
            c->col[1].r = 0x80;
            c->col[1].g = 0x80;
            c->col[1].b = 0x80;
            c->col[1].a = 0x80;
            c->col[2].r = 0x80;
            c->col[2].g = 0x80;
            c->col[2].b = 0x80;
            c->col[2].a = 0x80;
            c->col[3].r = 0x80;
            c->col[3].g = 0x80;
            c->col[3].b = 0x80;
            c->col[3].a = 0x80;
            c->type = 0xD;
            c->uv[0][0] = 0x8E;
            c->uv[0][1] = 0xDB;
            c->uv[1][0] = 0x8E;
            c->uv[1][1] = 0xEB;
            c->uv[2][0] = 0x9E;
            c->uv[2][1] = 0xDB;
            c->uv[3][0] = 0x9E;
            c->uv[3][1] = 0xEB;
            vec[0] = p[0] / 16000.0f * 62.5f;
            vec[1] = 0;
            vec[2] = p[2] / 16000.0f * 62.5f;
            vec[3] = 0;
            GphQuad3DSetCorners(c, m1, vec, mapMiniWork->iconScale, mapMiniWork->iconScale);
            c++;
        }
    }
    pl = ChrGetByNo(1);
    c->col[0].r = 0x80;
    c->col[0].g = 0x80;
    c->col[0].b = 0x80;
    c->col[0].a = 0x80;
    c->col[1].r = 0x80;
    c->col[1].g = 0x80;
    c->col[1].b = 0x80;
    c->col[1].a = 0x80;
    c->col[2].r = 0x80;
    c->col[2].g = 0x80;
    c->col[2].b = 0x80;
    c->col[2].a = 0x80;
    c->col[3].r = 0x80;
    c->col[3].g = 0x80;
    c->col[3].b = 0x80;
    c->col[3].a = 0x80;
    c->type = 0xD;
    c->uv[0][0] = 0xAA;
    c->uv[0][1] = 0xC9;
    c->uv[1][0] = 0xAA;
    c->uv[1][1] = 0xD9;
    c->uv[2][0] = 0xBA;
    c->uv[2][1] = 0xC9;
    c->uv[3][0] = 0xBA;
    c->uv[3][1] = 0xD9;
    vec[0] = pl->model->pos[0] / 16000.0f * 62.5f;
    vec[1] = 0;
    vec[2] = pl->model->pos[2] / 16000.0f * 62.5f;
    vec[3] = 0;
    GphQuad3DSetCorners(c, m2, vec, mapMiniWork->iconScale, mapMiniWork->iconScale);
    c++;
    c->type = 0;
    GphBatchSetOffset(0, 0);
    GphBatchDrawQuads3D(mapMiniWork->icons);
}
