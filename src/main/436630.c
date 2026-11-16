#include "common.h"
#include "main/n04.h"
#include "protos/main.h"

void GphDrawListDrawAll(void) {
    GphDrawList *p = gphDrawListCur;
    if (p->count != 0) {
        GphDrawListRender(p->keys, p->count);
    }
}
