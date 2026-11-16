/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n23.h"
#include "field/n24.h"

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
void VuBezier3(FVECTOR *, FVECTOR *, f32);

s32 FldMenuJunctionScatterBuild(s32 arg0) {
    FVECTOR v;
    FldMenuJunctionPiece (*base)[5];
    FldMenuQuad *spr;
    s32 i, j;
    s8 id;
    s32 x, y, w, h;

    if (FldMenuWinGetState(0xD) >= 4) {
        base = FldMenuGetParticleBuf();
        spr = FldMenuWinGetSprBuf(0xD, 3);
        FldMenuWinSetLayer(0xD, 3, &fldMenuTexIcons, spr, 128.0f);
        fldMenuJunction.flyT += 0.02f;
        if (fldMenuJunction.flyT >= 1.0f) {
            fldMenuJunction.flyT = 1.0f;
        }
        for (i = 0; i < itemWork->u6E20.junc.rows; i++) {
            for (j = 0; j < 5; j++) {
                VuBezier3(&v, base[i][j].pts, fldMenuJunction.flyT);
                id = itemWork->u6E20.junc.tbl[i][j];
                if (id != 0) {
                    spr->v[0].r = 0x80;
                    spr->v[0].g = 0x80;
                    spr->v[0].b = 0x80;
                    spr->v[0].a = 0;
                    spr->type = 1;
                    x = v[0];
                    y = v[1];
                    w = (s32)((s16)fldMenuJunctionIcons[id - 1].w * v[2]) + 320;
                    h = (s32)((s16)fldMenuJunctionIcons[id - 1].h * v[2]) + 64;
                    spr->v[0].x = x + 320;
                    spr->v[0].y = y + 64;
                    spr->v[1].x = x + w;
                    spr->v[1].y = y + h;
                    spr->v[0].u = fldMenuJunctionIcons[id - 1].u0;
                    spr->z = 0;
                    spr->v[0].v = fldMenuJunctionIcons[id - 1].v0;
                    spr->v[1].u = fldMenuJunctionIcons[id - 1].u1;
                    spr->v[1].v = fldMenuJunctionIcons[id - 1].v1;
                    spr++;
                }
            }
        }
        spr->type = 0;
    }
    return arg0;
}
