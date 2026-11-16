/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "gph.h"

void VuMtxSetRotZ(f32 (*)[4], s32);
void GphModelCalcCull(GphModel *, f32);

void BtlChrModelUpdate(BtlUnit *arg0) {
    VuVec4 v;
    f32 m[4][4];
    GphModel *o;
    BtlUnit *u;
    s32 i;

    if (btlChrModelUpdated[arg0->id] == 0) {
        if (arg0->model->motion[0].motion != arg0->motion) {
            arg0->model->motion[0].motion = arg0->motion;
            ChrModelClearDeformBufs(arg0->model);
        }
        if (arg0->motion != NULL) {
            if ((arg0->motion->playFlags & 2) && !(btlDebugCfg.flags & 0x80)) {
                ChrModelGetPartPos(&v, arg0->model, 0, arg0->frame);
                if (arg0->unk12C < 30000.0f) {
                    v.y = v.x - arg0->unk128;
                    v.w = -(v.z - arg0->unk12C);
                } else {
                    v.y = v.w = 0.0f;
                }
            } else {
                v.x = v.y = v.z = v.w = 0.0f;
            }
            arg0->unk128 = v.x;
            arg0->unk12C = v.z;
            VuMtxSetRotZ(m, arg0->rot[1]);
            BtlUnitMoveDelta(arg0, 0, -(m[0][0] * v.y + m[1][0] * v.w), m[0][1] * v.y + m[1][1] * v.w);
        }
        o = arg0->model;
        o->rot[0] = arg0->rot[0];
        o->pos[0] = arg0->pos[0] + arg0->unkD0[0];
        o->rot[1] = 0x8000 - arg0->rot[1];
        o->pos[1] = arg0->pos[1] + arg0->unkD0[1];
        o->rot[2] = arg0->rot[2];
        o->pos[2] = arg0->pos[2] + arg0->unkD0[2];
        o->scale[0] = arg0->scale[0];
        o->scale[1] = arg0->scale[1];
        o->scale[2] = arg0->scale[2];
        o->motion[0].frame = arg0->frame;
        GphModelCalcCull(o, 0.0f);
        btlChrModelUpdated[arg0->id] = 1;
        if (arg0->parts[0] >= 0 && arg0->state != 0) {
            for (i = 0; i < 3; i++) {
                if (arg0->parts[i] >= 0) {
                    u = &btlUnits[arg0->parts[i]];
                    BtlChrPartToWorld(u->pos, arg0, BtlChrFindPart(arg0->model, i * 1000 + 2001), 0);
                    u->rot[1] = arg0->rot[1];
                }
            }
        }
    }
}
