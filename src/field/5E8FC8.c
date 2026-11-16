#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrInstSetBlink(s32 arg0, s32 arg1, u16 arg2) {
    chrInstPool->slots[SysCommonModelGroup(arg0)][arg1].blink = arg2;
}
