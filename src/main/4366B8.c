#include "common.h"
#include "main/n04.h"
#include "protos/main.h"

void GphDrawListDrawAllNormLight(void) {
    GphDrawList *p = gphDrawListCur;
    s32 t = p->count;
    if (t != 0) {
        GphDrawListRenderNormLight(p->keys, t);
    }
}
