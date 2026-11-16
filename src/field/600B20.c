/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

void VuBezier3(FVECTOR *, FVECTOR *, f32);
void GphQuad3DSetCorners(GphQuad3D *, s32, FVECTOR *, f32, f32);

GphQuad3D *MapMiniTenantIconAdd(GphQuad3D *p, ChrObj *c, s32 arg2, u8 tex, u8 col) {
    f32 scale;
    FVECTOR v;
    FVECTOR pts[4];
    f32 y;
    u16 u0;
    u16 v0;

    scale = mapMiniWork->iconScale;
    p->type = 13;
    if (col == 0x80) {
        p->col[0].r = col;
        p->col[0].g = col;
        p->col[0].b = col;
        p->col[0].a = col;
        p->col[1].r = col;
        p->col[1].g = col;
        p->col[1].b = col;
        p->col[1].a = col;
        p->col[2].r = col;
        p->col[2].g = col;
        p->col[2].b = col;
        p->col[2].a = col;
        p->col[3].r = col;
        p->col[3].g = col;
        p->col[3].b = col;
        p->col[3].a = col;
        y = 1.0f;
    } else {
        p->col[0].r = col;
        p->col[0].g = 0;
        p->col[0].b = 0;
        p->col[0].a = 0x80;
        p->col[1].r = col;
        p->col[1].g = 0;
        p->col[1].b = 0;
        p->col[1].a = 0x80;
        p->col[2].r = col;
        p->col[2].g = 0;
        p->col[2].b = 0;
        p->col[2].a = 0x80;
        p->col[3].r = col;
        p->col[3].g = 0;
        p->col[3].b = 0;
        p->col[3].a = 0x80;
        pts[0][0] = 1.0f;
        pts[1][0] = 3.0f;
        pts[2][0] = 3.0f;
        pts[3][0] = 1.0f;
        VuBezier3(&v, pts, (col - 128.0f) * 0.0078125f);
        y = v[0];
    }
    v[0] = c->model->pos[0] / 16000.0f * 62.5f;
    v[1] = y;
    v[2] = c->model->pos[2] / 16000.0f * 62.5f;
    v[3] = 0.0f;
    if (tex != 0) {
        if (tex != 99) {
            u0 = menuHelpVsTypeIcons[tex].u0;
            v0 = menuHelpVsTypeIcons[tex].v0;
            p->uv[0][0] = u0;
            p->uv[0][1] = v0;
            p->uv[1][0] = u0;
            p->uv[1][1] = v0 + 24;
            p->uv[2][0] = u0 + 24;
            p->uv[2][1] = v0;
            p->uv[3][0] = u0 + 24;
            p->uv[3][1] = v0 + 24;
        }
    } else {
        p->uv[0][0] = 0;
        p->uv[0][1] = 0;
        p->uv[1][0] = 0;
        p->uv[1][1] = 24;
        p->uv[2][0] = 24;
        p->uv[2][1] = 0;
        p->uv[3][0] = 24;
        p->uv[3][1] = 24;
    }
    GphQuad3DSetCorners(p, arg2, &v, scale, scale);
    p++;
    p->type = 0;
    return p;
}
