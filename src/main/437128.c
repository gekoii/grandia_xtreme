#include "common.h"
#include "gph.h"

u16 ChrPartGetGaObjCount(GphPart *arg0) {
    if (arg0->gaObjs == NULL) {
        return 0;
    }
    return arg0->gaObjCount;
}
