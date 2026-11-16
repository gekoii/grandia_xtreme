/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

void VuSinCosDeg(f32 *, f32 *, f32);
void GphQuad3DSetCorners(GphQuad3D *, FVECTOR *, FVECTOR *, f32, f32);

void ChrStunStarsDraw(void) {
    f32 c;
    f32 s;
    GphQuad3D buf[9];
    FVECTOR v;
    FVECTOR mtx[4];
    ChrObj *ch;
    GphQuad3D *p;
    s32 i;
    f32 k;
    GphModel *o;

    ch = ChrGetByNo(1);
    if (ch == NULL || ch->stunState == 0) {
        return;
    }
    GphTexBind(0xF1);
    VuMtxSetRotXY(mtx, camField[0].pitch, camField[0].yaw, camField[0].roll);
    p = buf;
    k = FldMenuWobbleCurve(ch->stunSpin / 45.0f);
    for (i = 0; i < 8; i++) {
        VuSinCosDeg(&s, &c, i * 45.0f + ch->stunSpin * 2.0f);
        p->type = 13;
        switch (ch->stunState) {
        case 1:
            p->col[0].r = 0x80;
            p->col[0].g = 0x80;
            p->col[0].b = 0x80;
            p->col[0].a = ch->stunAnim * 4;
            p->col[1].r = 0x80;
            p->col[1].g = 0x80;
            p->col[1].b = 0x80;
            p->col[1].a = ch->stunAnim * 4;
            p->col[2].r = 0x80;
            p->col[2].g = 0x80;
            p->col[2].b = 0x80;
            p->col[2].a = ch->stunAnim * 4;
            p->col[3].r = 0x80;
            p->col[3].g = 0x80;
            p->col[3].b = 0x80;
            p->col[3].a = ch->stunAnim * 4;
            break;
        case 2:
            p->col[0].r = 0x80;
            p->col[0].g = 0x80;
            p->col[0].b = 0x80;
            p->col[0].a = 0x80;
            p->col[1].r = 0x80;
            p->col[1].g = 0x80;
            p->col[1].b = 0x80;
            p->col[1].a = 0x80;
            p->col[2].r = 0x80;
            p->col[2].g = 0x80;
            p->col[2].b = 0x80;
            p->col[2].a = 0x80;
            p->col[3].r = 0x80;
            p->col[3].g = 0x80;
            p->col[3].b = 0x80;
            p->col[3].a = 0x80;
            break;
        case 3:
            p->col[0].r = 0x80;
            p->col[0].g = 0x80;
            p->col[0].b = 0x80;
            p->col[0].a = 0x80 - ch->stunAnim * 4;
            p->col[1].r = 0x80;
            p->col[1].g = 0x80;
            p->col[1].b = 0x80;
            p->col[1].a = 0x80 - ch->stunAnim * 4;
            p->col[2].r = 0x80;
            p->col[2].g = 0x80;
            p->col[2].b = 0x80;
            p->col[2].a = 0x80 - ch->stunAnim * 4;
            p->col[3].r = 0x80;
            p->col[3].g = 0x80;
            p->col[3].b = 0x80;
            p->col[3].a = 0x80 - ch->stunAnim * 4;
            break;
        }
        o = ch->model;
        v[0] = o->pos[0] + c * 60.0f;
        v[2] = o->pos[2] + s * 60.0f;
        p->uv[0][0] = sprStarIconRects[0].u0;
        p->uv[0][1] = sprStarIconRects[0].v0;
        p->uv[1][0] = sprStarIconRects[0].u0;
        p->uv[1][1] = sprStarIconRects[0].v1;
        p->uv[2][0] = sprStarIconRects[0].u1;
        p->uv[2][1] = sprStarIconRects[0].v0;
        p->uv[3][0] = sprStarIconRects[0].u1;
        p->uv[3][1] = sprStarIconRects[0].v1;
        if (i & 1) {
            v[1] = o->pos[1] + 192.0f + k * 8.0f;
        } else {
            v[1] = o->pos[1] + 192.0f + k * -8.0f;
        }
        GphQuad3DSetCorners(p, mtx, &v, k * 4.0f + 8.0f, k * 4.0f + 8.0f);
        p++;
    }
    p->type = 0;
    GphBatchSetOffset(0, 0);
    GphBatchDrawQuads3D(buf);
}
