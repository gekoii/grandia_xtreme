#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrMotSetRandomFrame(ChrObj *o, u8 id) {
    if (ChrMotSet(o, id) != 0) {
        return -1;
    }
    o->model->motion[0].frame = __builtin_fabsf(VuRandSigned(o->model->motion[0].motion->frameCount - 1));
    return 0;
}
