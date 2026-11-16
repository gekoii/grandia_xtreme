/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "gph.h"

f32 VuSinAngle(s32);
void GphModelCalcCull(GphModel *, f32);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void FxCloneUpdate(void) {
    FxClone *g = &fxClone;
    FxCloneEnt *s;
    s32 ang;
    s32 a;
    s32 i;
    f32 dx, dz, r;

    if (g->unit == NULL) {
        return;
    }
    if (g->unk4C > 0) {
        if (--g->unk4C == 0) {
            g->frameStamp = btlFrameCount;
            return;
        }
    } else if (g->unk4C == 0) {
        if (g->frameStamp != btlFrameCount) {
            FxCloneFree();
        }
        return;
    }
    if (g->circleMode == 1) {
        ang = BtlAtan2Angle(g->unit->pos[0] - g->cx, g->unit->pos[2] - g->cz);
        a = ang;
        dx = g->unit->pos[0] - g->cx;
        dz = g->unit->pos[2] - g->cz;
        r = fsqrt(dx * dx + dz * dz);
        s = g->clones;
        for (i = 0; i < g->count; i++, s++) {
            a += 0x10000 / (g->count + 1);
            s->model->pos[0] = g->cx + VuSinAngle(a) * r;
            s->model->pos[1] = g->unit->pos[1];
            s->model->pos[2] = g->cz + VuCosAngle(a) * r;
            s->model->rot[0] = g->unit->rot[0];
            s->model->scale[0] = g->unit->scale[0];
            s->model->rot[1] = 0x8000 - (g->unit->rot[1] + a - ang);
            s->model->scale[1] = g->unit->scale[1];
            s->model->rot[2] = g->unit->rot[2];
            s->model->scale[2] = g->unit->scale[2];
        }
    }
    s = g->clones;
    for (i = 0; i < g->count; i++, s++) {
        if (s->step != 0.0f) {
            s->alpha += s->step;
            if ((s->step > 0.0f && s->target <= s->alpha) || (s->step < 0.0f && s->alpha <= s->target)) {
                s->alpha = s->target;
                s->step = 0.0f;
            }
        }
        if (s->model->motion[0].motion != g->unit->motion) {
            s->model->motion[0].motion = g->unit->motion;
            ChrModelClearDeformBufs(s->model);
        }
        s->model->motion[0].frame = g->unit->frame;
        GphModelCalcCull(s->model, 0.0f);
    }
}
