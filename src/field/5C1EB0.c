#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrTenantDebugDrawCamTargetArea(void) {
    s32 i;
    s32 n;
    s32 id;

    i = 0;
    n = ChrTenantGetCount();
    if (n > 0) {
        do {
            id = ChrTenantAt(i);
            i++;
            if (id == (s32)camCtl.target) {
                ChrTenantDebugDrawCurArea(id);
            }
        } while (i < n);
    }
}
