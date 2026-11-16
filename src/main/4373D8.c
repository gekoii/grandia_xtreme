#include "common.h"
#include "gph.h"

void ChrModelSetGaObjParam30(GphModel *arg0, f32 arg1) {
    s32 i;
    GphGaObject *e;

    if (arg0 != NULL) {
        e = arg0->gaObjs;
        for (i = arg0->gaObjCount; i > 0; i--) {
            e->unk30 = arg1;
            e++;
        }
    }
}
