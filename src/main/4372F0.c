#include "common.h"
#include "gph.h"

void ChrModelSetGaObjFlag2(GphModel *arg0, s32 arg1) {
    s32 i;
    GphGaObject *it;

    if (arg0 != NULL) {
        i = arg0->gaObjCount;
        it = arg0->gaObjs;
        if (arg1 != 0) {
            for (; i > 0; i--) {
                it->mode |= 2;
                it++;
            }
        } else {
            for (; i > 0; i--) {
                it->mode &= ~2;
                it++;
            }
        }
    }
}
