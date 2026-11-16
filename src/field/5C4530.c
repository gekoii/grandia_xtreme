#include "common.h"
#include "field/state.h"

s32 ChrTenantOpGroundOff(s32 arg0, ChrObj *obj) {
    obj->flags &= 0xFFFB;
    return 0;
}
