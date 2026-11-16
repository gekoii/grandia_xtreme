#include "common.h"
#include "main/n04.h"

s32 GphDrawListSet(s32 arg0) {
    gphDrawListCur = (GphDrawList *)arg0;
    return 0;
}

s32 GphDrawListGetCurrent(void) {
    return (s32)gphDrawListCur;
}

void GphDrawListClear(void *arg0) {
    GphDrawList *list;

    list = arg0;
    if (list == NULL) {
        list = gphDrawListCur;
    }
    list->count = 0;
    *list->envMapList = 0;
}
