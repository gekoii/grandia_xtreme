#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "protos/field.h"

extern void VuVecCopy(f32 *, s32);

void ChrInstSetPos(s32 arg0, s32 arg1, s32 arg2) {
    VuVecCopy(chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].model->pos, arg2);
}
