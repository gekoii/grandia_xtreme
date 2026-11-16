#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrMotSetImmediateOnce(ChrObj *o, u8 id) {
    s32 r;

    r = ChrMotSetImmediate(o, id);
    if (r == 0) {
        ChrMotFlagsSet((u32 *)o->model, 2, 0);
        ChrMotFlagsSet((u32 *)o->model, 4, 1);
    }
    return r;
}
