#include "common.h"
#include "main/n05.h"

void GphProjTexAddEntry(u8 *arg0, s32 arg1) {
    GphProjTexList *t = gphProjTexList;
    GphProjTexEntry *e = &t->entries[t->count];
    e->type = 1;
    e->mtx = arg1;
    e->gaObj = (GphGaObject *)arg0;
    e->layer = arg0[2];
    t->count++;
}
