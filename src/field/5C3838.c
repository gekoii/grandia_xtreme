#include "common.h"
#include "field/n14.h"

s32 ChrTenantOpSightOn(void *arg0, ChrObj *a) {
    CHR_SENSE(a)->flags |= 4;
    return 0;
}
