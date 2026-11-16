#include "common.h"
#include "field/state.h"

s32 ChrTenantOpGroundOn(s32 arg0, ChrObj *obj) {
    obj->flags |= 4;
    return 0;
}
