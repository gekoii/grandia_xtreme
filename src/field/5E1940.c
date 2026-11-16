#include "common.h"
#include "field/state.h"

void ChrTenantSetActive(ChrObj *arg0, u8 arg1) {
    if (arg1) {
        arg0->flags |= 2;
    } else if (arg0->flags & 2) {
        arg0->flags ^= 2;
    }
}
