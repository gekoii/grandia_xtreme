#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void PtclBurstSpawnAtUnit(s32 arg0, BtlUnit *arg1, s32 arg2) {
    VuVec3 v;
    f32 s, c;

    VuSinCosAngle(&s, &c, arg2);
    v.x = arg1->pos[0] + arg1->params->colR * 0.5f * s;
    v.y = BtlChrGetCenterY((s32)arg1);
    v.z = arg1->pos[2] + arg1->params->colR * 0.5f * c;
    PtclBurstSpawn(arg0, arg1, &v);
}
