#include "common.h"
#include "field/state.h"

s32 ChrTenantOpEnd(void *arg0, ChrObj *a) {
    a->headingDeg = a->heading * 0.0054931640625f;
    return 0;
}
