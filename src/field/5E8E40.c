#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrInstSetSpin(s32 arg0, s32 arg1) {
    chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].spin = 1;
}
