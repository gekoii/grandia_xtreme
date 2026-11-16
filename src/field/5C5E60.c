/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "gph.h"
#include "field/n14.h"

void VuBezier3(VuVec4 *, VuVec4 *, f32);
void VuProjectVerts(VuVec4 *, void *, VuVec4 *, s32);
void VuVecLerp(VuVec4 *, VuVec4 *, VuVec4 *, f32);
s32 fptoui(f32);

void FldDmgPopUpdate(FldHpGauge *tm) {
    VuVec4 scr[2];
    VuVec4 pos;
    VuVec4 out;
    VuVec4 a;
    VuVec4 b;
    VuVec4 k[4];
    ChrObj *pl;
    void *cam;
    GphBatchQuad *p;
    GphBatchQuad *bar;
    s32 i;
    f32 y;
    f32 s;

    fldDmgPopWork->count = 0;
    tm->cur = ptyPartyUnits->hp;
    pl = ChrGetByNo(1);
    if (pl == NULL) {
        return;
    }
    cam = VuMtxStackGetTop();
    p = fldDmgPopWork->bars;
    p->type = 0;
    bar = p++;
    p = bar + 4;
    bar[4].type = 0;
    for (i = 0; i < 32; i++) {
        if (fldDmgPopWork->active[i] == 0) {
            continue;
        }
        pos.x = fldDmgPopWork->pos[i][0];
        pos.y = y = fldDmgPopWork->pos[i][1] + 180.0f;
        pos.z = fldDmgPopWork->pos[i][2];
        pos.w = 0.0f;
        if (1.0f <= fldDmgPopWork->t[i]) {
            k[0].x = y;
            k[1].x = y - 16.0f;
            k[2].x = y;
            k[3].x = y;
            k[0].y = 2.5f;
            k[1].y = 2.0f;
            k[2].y = 2.0f;
            k[3].y = 2.5f;
            k[0].z = 128.0f;
            k[1].z = 128.0f;
            k[2].z = 128.0f;
            k[3].z = 0.0f;
            VuBezier3(&out, k, fldDmgPopWork->t[i] - 1.0f);
        } else {
            k[0].x = y - 64.0f;
            k[1].x = y + 32.0f;
            k[2].x = y + 64.0f;
            k[3].x = y;
            k[0].y = 1.0f;
            k[1].y = 1.2f;
            k[2].y = 1.8f;
            k[3].y = 2.5f;
            k[0].z = 0.0f;
            k[1].z = 128.0f;
            k[2].z = 128.0f;
            k[3].z = 128.0f;
            VuBezier3(&out, k, fldDmgPopWork->t[i]);
        }
        pos.y = out.x;
        VuProjectVerts(scr, cam, &pos, 2);
        if (0.0f <= scr[0].z) {
            p = FldDmgNumBuildSprites(p, scr[0].x, scr[0].y, fldDmgPopWork->value[i], out.z, out.y, out.y);
        }
        p->type = 0;
        if (1.0f <= fldDmgPopWork->t[i]) {
            fldDmgPopWork->t[i] += 0.03f;
        } else {
            fldDmgPopWork->t[i] += 0.05f;
        }
        if (2.0f <= fldDmgPopWork->t[i]) {
            fldDmgPopWork->active[i] = 0;
            fldDmgPopWork->t[i] = 0.0f;
        }
        fldDmgPopWork->count++;
    }
    if (tm->cur == tm->shown) {
        tm->flag = 0;
        tm->animT = 0.0f;
        return;
    }
    pos.x = pl->model->pos[0];
    pos.y = pl->model->pos[1] + 64.0f;
    pos.z = pl->model->pos[2];
    pos.w = 0.0f;
    VuProjectVerts(scr, cam, &pos, 2);
    if (tm->flag != 0) {
        return;
    }
    tm->animT += 0.1f;
    a.z = (f32)tm->shown / tm->max * 84.0f;
    b.z = (f32)tm->cur / tm->max * 84.0f;
    s = (1.0f < tm->animT) ? 1.0f : tm->animT;
    VuVecLerp(&out, &a, &b, s);
    p = fldDmgPopWork->bars;
    if (tm->shown > tm->cur) {
        p = FldHpBarBuildSprites(p, fptoui(84.0f - a.z), fptoui(84.0f - b.z), fptoui(84.0f - out.z), scr[0].x - 84.0f, scr[0].y);
    } else {
        p = FldHpBarBuildSprites(p, fptoui(a.z), 0, fptoui(84.0f - out.z), scr[0].x, scr[0].y);
    }
    p->type = 0;
    fldDmgPopWork->count += 3;
    if (6.0f < tm->animT) {
        tm->flag = 0xFF;
        tm->shown = tm->cur;
    }
}
