/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"
#include "field/n15.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    f32 x, y, z, w;
} Vec __attribute__((aligned(16)));

void VuVecCopy(void *, void *);
void CamStateCalcEye(CamState *);
void CamAiPresetUpdate(Camera *, Vec *);

void CamFollowUpdate(Camera *o, u32 mode) {
    Vec v;
    Vec tmp;
    CamCtl *c = &camCtl;
    Pad *pad;
    f32 t;
    GphModel *mdl;

    if (c->target == NULL) {
        return;
    }
    CamDebugTriClear();
    MapWallSelectLayer(1);
    pad = padPort0;
    /* goto layout inside a do-while(0): the mode >= 2 test sits after the mode 3 and
       other blocks, as the original lays them out (if/else chains: 169-305 words off) */
    do {
        if (mode == 2) {
            c->height = c->target->model->cullSphere[3] * 0.5f;
        } else {
            goto test;
        big:
            if (mode == 3) {
                GphModel *m = c->target->model;
                f32 r = m->motion[0].frame;
                f32 h = 0.0f;

                ChrModelGetPartPosById(&tmp, m, 1000, r);
                h += tmp.y;
                ChrModelGetPartPosById(&tmp, m, 100, r);
                h += tmp.y;
                ChrModelGetPartPosById(&tmp, m, 220, r);
                h += tmp.y;
                ChrModelGetPartPosById(&tmp, m, 520, r);
                h += tmp.y;
                ChrModelGetPartPosById(&tmp, m, 640, r);
                h += tmp.y + 60.0f;
                c->height = h;
            } else {
                if (mode != 4 && mode != 13) {
                    o->fov = 42.0f;
                }
                camCtl.height = camCtl.target->model->cullSphere[3] - 20.0f;
                if (mode == 4) {
                    camCtl.height = camCtl.unk14;
                }
            }
            goto out;
        test:
            if (mode >= 2) goto big;
            c->height = (c->target->model->cullSphere[3] - 20.0f) * 2.0f;
        }
    out:;
    } while (0);
    VuVecCopy(&v, &camCtl.target->model->pos);
    v.y += camCtl.height;
    if (mode == 0 || mode == 3) {
        copyq(camFollow->cur.lookAt, o->lookAt);
        copyq(camFollow->cur.eye, o->eye);
        copyq(camFollow->cur.dir, o->dir);
        camFollow->cur.dist = o->dist;
        camFollow->cur.pitch = o->pitch;
        camFollow->cur.yaw = o->yaw;
        camFollow->save = camFollow->cur;
        VuVecCopy(camFollow->save.lookAt, &v);
        t = CamPadTurnGet(o, pad, 0, 0.93f, 0.0f) * 0.017453292f * camFollow->rate[0];
        camFollow->save.pitch = camFollow->pitch;
        camFollow->save.dist = camFollow->dist[CAM_FOLLOW_DIST_NORMAL];
        camFollow->turn = t;
        camFollow->save.yaw += t;
        CamStateCalcEye(&camFollow->save);
        CamCalcEyePos(&v, (Vec *)camFollow->unk90, camFollow->save.yaw, camFollow->pitch, camFollow->cur.dist);
        VuVecCopy(camFollow->unk60, &v);
        if ((pad->trigger & 2) || camCtl.resetBehind) {
            camCtl.resetBehind = 0;
            CamFollowResetBehind(o, &v);
            return;
        }
    }
    switch (mode) {
    case 1:
        CamAiGrandia2(o, camCtl.target, pad);
        break;
    case 2:
        CamAiEye(o, camCtl.target, pad);
        break;
    case 3:
        CamAiLadder(o, camCtl.target, pad);
        break;
    case 4:
        CamAiFF(o, camCtl.target, pad);
        break;
    case 5:
        CamAiTown(o, camCtl.target, pad);
        break;
    case 0:
    case 6:
    case 8:
    case 10:
        CamAiFollowUpdate(o, &v);
        break;
    case 7:
    case 9:
    case 11:
        CamAiPresetUpdate(o, &v);
        break;
    case 12:
        CamAiFree(o, &v);
        break;
    }
    if (camCtl.aiType == 0) {
        if (camCtl.target != NULL) {
            mdl = camCtl.target->model;
            if (o->dist < 60.0f) {
                mdl->flags |= 8;
                camCtl.playerShown = 0;
            } else {
                mdl->flags &= ~8;
                camCtl.playerShown = 1;
            }
        }
    } else {
        camCtl.playerShown = 1;
    }
    if (mode == 3 && camCtl.target != NULL) {
        mdl = camCtl.target->model;
        mdl->flags &= ~8;
    }
    copyq(camFollow->unk70, &v);
}
