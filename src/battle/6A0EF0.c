/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006E0E20-0x006E0E34 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "party.h"

#define RND() (rand() >> 15)


void FxCastAuraUpdate(BtlUnit *u) {
    FxCastAura *p = &fxCastAura[u->id];
    VuVec4 v;
    f32 s;
    s32 n;
    GphPart *o;
    GphModel *e;

    switch (p->state) {
    case 0:
        if (p->stamp == btlFrameCount) {
            break;
        }
        if (p->pool != 0) {
            MemFree(p->pool);
        }
        p->model = BtlModelCreateWithPool(p->kind == 1 ? fxCastAuraModelMove : fxCastAuraModelMagic, &p->pool);
        if (p->model == NULL) {
            FxCastAuraFree(p);
            return;
        }
        p->model->rot[0] = RND();
        p->model->rot[1] = RND();
        p->model->rot[2] = RND();
        e = p->model;
        s = u->params->colR * 0.016666668f;
        if (s < 0.7f) {
            s = 0.7f;
        }
        e->scale[2] = s;
        e->scale[1] = s;
        e->scale[0] = s;
        if (u->id < 4) {
            for (n = e->partCount, o = e->parts; n > 0; n--, o++) {
                o->flags &= ~8;
            }
        }
        p->alpha = p->u = p->v = 0.0f;
        p->state++;
        break;
    case 1:
        if (p->alpha < 1.0f) {
            p->alpha += 0.03125f;
            if (p->alpha > 1.0f) {
                p->alpha = 1.0f;
            }
        }
        if (p->release == 1) {
            p->state = 2;
        } else if (p->release == 2) {
            p->model->motion[0].motion = fxCastAuraBurstMotion;
            ChrModelClearDeformBufs(p->model);
            p->timer = 0;
            p->state = 3;
        }
        break;
    case 2:
        p->alpha -= 0.03125f;
        if (p->alpha <= 0.0f) {
            p->state = 4;
            p->stamp = btlFrameCount;
            p->model = NULL;
        }
        break;
    case 3:
        if (p->timer < 11) {
            BtlChrGetPartPos(&v, u, 1);
            FxCastSparkSpawn(&v);
            FxCastSparkSpawn(&v);
            FxCastSparkSpawn(&v);
            FxCastSparkSpawn(&v);
            FxCastSparkSpawn(&v);
        }
        ChrModelUpdate(p->model, 1.0f);
        if (p->model->motion[0].motion->frameCount - 1 <= p->model->motion[0].frame) {
            p->state = 4;
            p->stamp = btlFrameCount;
            p->model = NULL;
        }
        break;
    case 4:
        if (p->stamp != btlFrameCount) {
            FxCastAuraFree(p);
        }
        break;
    }
    if (p->model != NULL) {
        if (p->kind == 1) {
            p->u += (2.0f - u->ipPos) * 64.0f;
            BtlChrPartSetUvOffset(BtlChrFindPart(p->model, 1), p->u, 0);
        } else {
            p->u += 25.6f;
            p->v += (2.0f - u->ipPos) * 25.6f;
            BtlChrPartSetUvOffset(BtlChrFindPart(p->model, 1), p->u, p->v);
            BtlChrPartSetUvOffset(BtlChrFindPart(p->model, 2), p->u, p->v);
            BtlChrPartSetUvOffset(BtlChrFindPart(p->model, 3), p->u, p->v);
        }
    }
    if (p->release == 0) {
        if (u->ipMode < 2 || btlPhase != 0x80) {
            p->release = 1;
        }
    }
    p->timer++;
}
