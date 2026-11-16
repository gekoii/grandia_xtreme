#include "common.h"
#include "battle/env.h"
#include "sys.h"
#include "protos/battle.h"

void BtlDirFromPitchYaw(s32, s32, FVECTOR *);

void EnvLightCalc(void) {
    EnvLight *l = &envLight;
    EnvLight *l2 = (EnvLight *)&l->amb[1];
    s32 m = 140;
    EnvParam *e = EnvParamGet(btlEntryMapNo);

    l->amb[0][0] = e->amb[0][0] + (50.0f - e->amb[0][0]) * envContrastAmb;
    l->amb[0][1] = e->amb[0][1] + (50.0f - e->amb[0][1]) * envContrastAmb;
    l->amb[0][2] = e->amb[0][2] + (50.0f - e->amb[0][2]) * envContrastAmb;
    l->amb[0][3] = e->amb[0][3];

    l->intensity[0][0] = e->lights[0].intensity;
    l->color[0][0][0] = e->lights[0].color[0] + (f32)(m - e->lights[0].color[0]) * envContrastDir;
    l->color[0][0][1] = e->lights[0].color[1] + (f32)(m - e->lights[0].color[1]) * envContrastDir;
    l->color[0][0][2] = e->lights[0].color[2] + (f32)(m - e->lights[0].color[2]) * envContrastDir;
    l->color[0][0][3] = e->lights[0].color[3];
    l->angles[0].x[0] = e->lights[0].angle[0];
    l->angles[0].y[0] = e->lights[0].angle[1];
    BtlDirFromPitchYaw(l->angles[0].x[0], l->angles[0].y[0], &l->dir[0][0]);

    l->intensity[0][1] = e->lights[2].intensity;
    l2->color[0][0][0] = e->lights[2].color[0] + (f32)(m - e->lights[2].color[0]) * envContrastDir;
    l2->color[0][0][1] = e->lights[2].color[1] + (f32)(m - e->lights[2].color[1]) * envContrastDir;
    l2->color[0][0][2] = e->lights[2].color[2] + (f32)(m - e->lights[2].color[2]) * envContrastDir;
    l2->color[0][0][3] = e->lights[2].color[3];
    l->angles[0].x[1] = e->lights[2].angle[0];
    l->angles[0].y[1] = e->lights[2].angle[1];
    BtlDirFromPitchYaw(l->angles[0].x[1], l->angles[0].y[1], &l->dir[0][1]);

    l->amb[1][0] = e->amb[1][0];
    l->amb[1][1] = e->amb[1][1];
    l->amb[1][2] = e->amb[1][2];
    l->amb[1][3] = e->amb[1][3];

    l->intensity[1][0] = e->lights[1].intensity;
    l->color[1][0][0] = e->lights[1].color[0];
    l->color[1][0][1] = e->lights[1].color[1];
    l->color[1][0][2] = e->lights[1].color[2];
    l->color[1][0][3] = e->lights[1].color[3];
    l->angles[1].x[0] = e->lights[1].angle[0];
    l->angles[1].y[0] = e->lights[1].angle[1];
    BtlDirFromPitchYaw(l->angles[1].x[0], l->angles[1].y[0], &l->dir[1][0]);

    l->intensity[1][1] = e->lights[3].intensity;
    l2->color[1][0][0] = e->lights[3].color[0];
    l2->color[1][0][1] = e->lights[3].color[1];
    l2->color[1][0][2] = e->lights[3].color[2];
    l2->color[1][0][3] = e->lights[3].color[3];
    l->angles[1].x[1] = e->lights[3].angle[0];
    l->angles[1].y[1] = e->lights[3].angle[1];
    BtlDirFromPitchYaw(l->angles[1].x[1], l->angles[1].y[1], &l->dir[1][1]);
}
