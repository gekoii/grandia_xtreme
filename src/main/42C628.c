#include "common.h"
#include "main/n03.h"

void McSetSaveData(void *arg0, s32 arg1) {
    McFileEntry *p = &mcSaveFileEntry;
    p->data = arg0;
    p->size = arg1;
}
