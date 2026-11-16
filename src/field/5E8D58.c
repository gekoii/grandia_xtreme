#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "protos/field.h"

void VuVecCopy(void *, void *);

s32 ChrInstSpawn(s32 arg0, void *arg1, s32 arg2) {
    u16 g = SysCommonModelGroup(arg0);
    s32 r = ChrInstAlloc(arg0);

    if (r == -1) {
        return -1;
    }
    VuVecCopy(chrInstPool->slots[g][r].model->pos, arg1);
    return r;
}
