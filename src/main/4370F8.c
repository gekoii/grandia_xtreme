#include "common.h"
#include "gph.h"

GphGaObject *ChrPartGetGaObj(GphPart *p, s32 arg1) {
    GphGaObject *objs;

    objs = p->gaObjs;
    if ((objs == NULL) || (arg1 >= (s32)p->gaObjCount)) {
        return NULL;
    }
    return &objs[arg1];
}
