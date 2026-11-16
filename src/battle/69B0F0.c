/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"

f32 VuSinAngle(s32);

void FxDelayedCastBuild(FxAtk *e) {
    FxDelayedCast *o = e->work;
    VuVec3 v;
    f32 k;
    f32 s;
    f32 t;
    GphModel *m;

    if (o->model == NULL) {
        return;
    }
    ChrModelSetAlpha(o->model, -EnvStageShadeGet() * 128.0f);
    o->model->pos[0] = o->unit.pos[0];
    o->model->pos[1] = o->unit.pos[1];
    o->model->pos[2] = o->unit.pos[2];
    if (o->unit.params->id == 0xD1) {
        ChrModelUpdate(o->model, 1.0f);
    } else {
        if (o->state == 0) {
            if (o->timer >= 0x200) {
                k = 1.0f;
            } else {
                k = o->timer * (1.0f / 512.0f);
            }
        } else {
            k = 0.0f;
        }
        o->pulse = o->pulse + 65536.0f / (k * 128.0f + 16.0f);
        s = (1.3f - k * 0.3f) + VuSinAngle(o->pulse) * 0.05f;
        if (k < 1.0f) {
            if (o->timer < 0x80 && (btlTickCount & 2)) {
                v.z = -64.0f;
                v.y = -64.0f;
                v.x = -64.0f;
            } else {
                t = s + 0.05f;
                v.x = t * 96.0f * (1.0f - k) + 0.0f;
                v.y = 0.0f - t * 48.0f * (1.0f - k);
                v.z = 0.0f - t * 48.0f * (1.0f - k);
            }
            ChrModelSetColor(o->model, &v);
        }
        m = o->model;
        m->scale[0] = m->scale[1] = m->scale[2] = s;
        m->pos[1] -= 5.0f;
    }
    BtlChrModelCalcDraw(o->model);
}
