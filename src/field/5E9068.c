#include "common.h"
#include "field/state.h"
#include "protos/field.h"

u8 ChrInstIsVisible(s32 arg0, s32 arg1) {
    u16 g = SysCommonModelGroup(arg0);

    return chrInstPool->slots[g][arg1].visible;
}
