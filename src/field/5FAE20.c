#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrAllDeactivate(void) {
    s32 n;
    s32 i;
    ChrObj *c;

    n = ChrGetMaxCount();
    for (i = 0; i < n; i++) {
        c = ChrGetByNo(i + 1);
        if (c != NULL && (c->flags & 1)) {
            c->flags ^= 1;
        }
    }
}
