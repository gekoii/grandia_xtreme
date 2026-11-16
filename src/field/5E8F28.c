#include "common.h"
#include "field/state.h"
#include "protos/field.h"

s32 ChrInstGetModel(s32 arg0, s32 arg1) {
    u16 g = SysCommonModelGroup(arg0);

    return (s32)chrInstPool->slots[g][arg1].model;
}
