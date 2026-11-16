#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrInstFree(s32 arg0, s32 arg1) {
    chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].used = 0;
}
