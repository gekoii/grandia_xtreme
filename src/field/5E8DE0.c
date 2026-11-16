#include "common.h"
#include "protos/field.h"
#include "field/state.h"


GphPart *ChrInstGetPart(s32 arg0, s32 arg1, s32 arg2) {
    u16 g = SysCommonModelGroup(arg0);

    return ChrModelPartAt(chrInstPool->slots[g][arg1].model, arg2);
}
