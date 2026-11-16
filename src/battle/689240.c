/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "gph.h"
#include "battle/n28.h"


void FxHitSlotUpdate(FxHitSlot *e) {
    GphModel *o;
    GphPart *s;
    s32 n;
    s32 r;
    f32 fr;

    switch (e->state) {
    case 0:
        if (e->frameStamp == btlFrameCount) {
            return;
        }
        if (e->modelBuf != 0) {
            MemFree(e->modelBuf);
        }
        switch (e->type & 0xF0) {
        case 0x40:
            e->model = BtlModelCreateWithPool(fxHitFlashModel, &e->modelBuf);
            break;
        case 0x50:
            e->model = BtlModelCreateWithPool(fxHitSkillModel, &e->modelBuf);
            break;
        case 0x30:
            e->model = BtlModelCreateWithPool(fxHitCounterModel, &e->modelBuf);
            break;
        default:
            e->model = BtlModelCreateWithPool(fxHitModel, &e->modelBuf);
            break;
        }
        if (e->model == NULL) {
            FxHitFree(e);
            return;
        }
        e->model->pos[0] = e->pos[0];
        e->model->pos[1] = e->pos[1];
        e->model->pos[2] = e->pos[2];
        e->model->rot[0] = rand() >> 15;
        e->model->rot[1] = rand() >> 15;
        e->model->rot[2] = rand() >> 15;
        switch (e->type & 0xF0) {
        case 0x40:
            e->model->motion[0].motion = fxHitFlashMotion;
            ChrModelClearDeformBufs(e->model);
            e->model->rot[0] = 0xC000;
            e->model->flags |= 0x8000;
            e->model->rot[1] = 0x8000 - e->angleY;
            break;
        case 0x50:
            e->model->motion[0].motion = fxHitSkillMotion;
            ChrModelClearDeformBufs(e->model);
            break;
        case 0x30:
            e->model->motion[0].motion = fxHitCounterMotion;
            ChrModelClearDeformBufs(e->model);
            e->model->rot[0] = e->angleX;
            e->model->flags |= 0x8000;
            e->model->rot[1] = 0x8000 - e->angleY;
            break;
        case 0x10:
            e->model->motion[0].motion = fxHitMotion;
            ChrModelClearDeformBufs(e->model);
            e->model->scale[0] = e->model->scale[1] = e->model->scale[2] = 0.8f;
            break;
        case 0x20:
            e->model->motion[0].motion = fxHitMotion;
            ChrModelClearDeformBufs(e->model);
            o = e->model;
            n = o->partCount;
            s = o->parts;
            for (; n > 0; n--, s++) {
                s->flags &= ~8;
            }
            break;
        }
        r = EnvLightAlloc(0x830000);
        e->light = r;
        /* EnvLightSetPos called as taking an s8 id (one instruction fewer than
         * the s32 prototype); the colours are read as (&fxHitLightColors[i])->r
         * (fxHitLightColors[i].r schedules differently). */
        ((void (*)(s8, f32, f32, f32))EnvLightSetPos)(r, e->pos[0], e->pos[1], e->pos[2]);
        EnvLightSetColor(e->light, (&fxHitLightColors[e->type & 0xF])->r, (&fxHitLightColors[e->type & 0xF])->g, (&fxHitLightColors[e->type & 0xF])->b, 0.0f);
        EnvLightSetRange(e->light, 256.0f);
        e->state++;
        break;
    case 1:
        ChrModelUpdate(e->model, 1.0f);
        fr = e->model->motion[0].motion->frameCount - 1;
        if (fr <= e->model->motion[0].frame) {
            e->model = NULL;
            EnvLightFree(e->light);
            e->light = -1;
            e->state++;
            e->frameStamp = btlFrameCount;
        } else {
            EnvLightSetIntensity(e->light, 1.0f - e->model->motion[0].frame / fr);
        }
        break;
    case 2:
        if (e->frameStamp != btlFrameCount) {
            FxHitFree(e);
        }
        break;
    }
}
