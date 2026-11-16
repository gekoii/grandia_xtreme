/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "camera.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "protos/battle.h"

void VuMtxStackPush(u32);
void VuMtxMul(s32, s32, FVECTOR *);
void VuVecNormalize(VuVec4 *, VuVec4 *);

void EnvShadowBuild(void) {
    VuVec4 v;
    BtlUnit *u;
    BtlUnit *u2;
    s32 *h;
    s32 i;
    s32 j;
    EnvLight *cfg;
    FVECTOR *cam;
    GphLight *l;
    s32 *flags;
    s32 idx;
    f32 lim;
    f32 y;
    s32 m;

    if (btlCamera->eye[1] <= 0.0f) {
        return;
    }
    VuMtxStackPush(0);
    m = VuMtxStackGetTop();
    VuMtxMul(m, m, envShadowMtx);
    for (i = 0; i < 16; i++) {
        if (envShadowPreCb[i] != NULL) {
            envShadowPreCb[i](envShadowCbArg[i]);
        }
    }
    if (envShadowLightBlend == 0.0f) {
        u = btlUnits;
        for (j = 0; j < 16; j++, u++) {
            if (u->state > 0 && u->model != 0 && u->alpha > 0.0f && !(u->params->status & 1)) {
                GphDrawListSet(envShadowDrawLists[j]);
                GphDrawListClear(0);
                GphModelDrawSimple(u->model);
                GphDrawListIndex();
            }
        }
        VuMtxStackPop();
        return;
    }
    VuMtxStackPop();
    cfg = &envLight;
    u2 = btlUnits;
    lim = 0.01f;
    flags = cfg->lightFlags;
    h = envShadowDrawLists;
    for (i = 15; i >= 0; h++, i--, u2++) {
        cam = envShadowMtx;
        if (u2->state > 0 && u2->model != 0 && u2->alpha > 0.0f && !(u2->params->status & 1)) {
            GphDrawListSet(*h);
            GphDrawListClear(0);
            idx = envShadowLightIdx;
            if (flags[idx] & 0x400000) {
                l = GphLightGet(idx);
                v.x = l->dir[0];
                v.y = l->dir[1];
                v.z = l->dir[2];
            } else {
                l = GphLightGet(idx);
                v.x = u2->pos[0] - l->dir[0];
                v.y = u2->pos[1] - l->dir[1];
                v.z = u2->pos[2] - l->dir[2];
            }
            VuVecNormalize(&v, &v);
            v.y = (v.y - cfg->dir[0][0][1]) * envShadowLightBlend + cfg->dir[0][0][1];
            y = (v.x - cfg->dir[0][0][0]) * envShadowLightBlend + cfg->dir[0][0][0];
            v.x = y;
            v.z = (v.z - cfg->dir[0][0][2]) * envShadowLightBlend + cfg->dir[0][0][2];
            if (v.y >= 0.0f) {
                continue;
            }
            if (((v.y < 0.0f) ? -v.y : v.y) <= lim) {
                v.y = -0.01f;
            }
            y = v.y;
            cam[1][0] = -v.x / y;
            cam[1][2] = -v.z / y;
            VuMtxStackPush(0);
            m = VuMtxStackGetTop();
            VuMtxMul(m, m, cam);
            GphModelDrawSimple(u2->model);
            VuMtxStackPop();
            GphDrawListIndex();
        }
    }
    envShadowMtx[1][0] = envLight.dir[0][0][0] / ((envLight.dir[0][0][1] < 0.0f) ? -envLight.dir[0][0][1] : envLight.dir[0][0][1]);
    envShadowMtx[1][2] = envLight.dir[0][0][2] / ((envLight.dir[0][0][1] < 0.0f) ? -envLight.dir[0][0][1] : envLight.dir[0][0][1]);
}
