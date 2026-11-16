#include "common.h"
#include "field/state.h"

s32 ChrTenantOpLoop(s32 arg0, ChrObj *obj) {
    obj->opPc = 0;
    return 1;
}
