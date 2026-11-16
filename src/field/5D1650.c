#include "common.h"
#include "field/n15.h"

void MapEnemyTableLoad(void *arg0) {
    MapOffsetList *p;
    u32 n;
    u32 i;
    p = mapCharPackList;
    fldCharDefTable = arg0;
    n = p->count;
    i = 0;
    while (i < n) i++;
}
