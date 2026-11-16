#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrInstSetVisible(s32 arg0, s32 arg1, u8 arg2) {
    chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].visible = arg2;
}
