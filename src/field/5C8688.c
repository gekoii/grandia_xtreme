#include "common.h"
#include "gph.h"
#include "field/state.h"

s32 ChrObjSetModel(ChrObj *o, GphModel *s) {
    if (o == NULL) {
        return 0;
    }
    if (s == NULL) {
        return 0;
    }
    o->model = s;
    s->flags = 0;
    o->radius = 40.0f;
    return 1;
}
