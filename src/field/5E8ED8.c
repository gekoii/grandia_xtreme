#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "protos/field.h"

s32 ChrInstGetPos(s32 arg0, s32 arg1) {
    return (s32)chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].model->pos;
}
